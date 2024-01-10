
# Hayden Allen Typed-Space (HATS)
## What is HATS?
*The devil wears many HATS. Incorrect math is no longer one of them.*

HATS is a header-only C++ linear algebra library made for games and other real-time simulations. It provides data types for the 4D homogeneous coordinate system typically used in games, and aims to provide a higher level of mathematical abstraction than other similar math libraries (e.g. [glm](https://github.com/g-truc/glm)) without any runtime overhead.

## Usage
Here is an example of a simple 3D camera controlled by WASD/Space/Shift and the mouse
> ```cpp
> #include "hats/hats.h"
> using namespace hats;
> //
> // outside the main loop
> //
> float delta_time = 0.f;	// time since last frame
> // camera state
> float cam_angle_x = 0.f, cam_angle_y = 0.f, cam_speed = 1.f;
> point<space::WORLD> cam_pos(0, 0, 5);
> // create view matrix from given camera origin
> tmat<space::WORLD, space::CAMERA> view = tmat_util::translation<space::WORLD, space::CAMERA>(cam_pos);
> // create camera's perspective projection matrix from arbitrary settings
> pmat<space::CAMERA, space::CLIP>& proj = pmat_util::projection(fov_y, aspect_ratio, near, far);
> 
> //
> // within the main loop
> //
> // mouse controls camera rotation
> cam_angle_x += mouse_delta.x;
> cam_angle_y += mouse_delta.y;
> // keyboard controls camera translation
> // (direction constructor creates a unit vector)
> const direction<space::CAMERA> move_dir(
> 	is_key_pressed(KEY_D) 		- is_key_pressed(KEY_A),
> 	is_key_pressed(KEY_SPACE) 	- is_key_pressed(KEY_SHIFT),
> 	is_key_pressed(KEY_S) 		- is_key_pressed(KEY_W)
> );
> const vec<space::CAMERA> xz_camera(amount.x, 0, amount.z); // WASD move relative to camera orientation
> // (inverting a matrix swaps its `FROM` and `TO` spaces)
> const tmat<space::CAMERA, space::WORLD>& cam2world = view.invert_copy();
> // (transforming a vec<FROM> by a mat<FROM, TO> produces a vec<TO>)
> const vec<space::WORLD> xz_world = cam2world * xz_camera;
> const vec<space::WORLD> y_world(0, amount.y, 0); // space/shift always move along world y-axis
> // (operator overloads let you write compact math)
> cam_pos += (xz_world + y_world) * cam_speed * delta_time; // combine to get total movement
> 
> // update view matrix based on input
> // (tmat_util helper functions create "pure" operation matrices)
> view = tmat_util::translation<space::WORLD, space::CAMERA>(cam_pos); // create camera's coordinate frame centered at world-space position
> view = tmat_util::rotation_y<space::CAMERA>(m_angle_y) * view; // rotate about camera's y-axis first
> view = tmat_util::rotation_x<space::CAMERA>(m_angle_x) * view; // then rotate about camera's x-axis
> 
> // construct view-projection matrix for this frame
> const mat<space::WORLD, space::CLIP>& view_proj = proj * view;
> 
> // shader setup pseudocode
> // (use mat.e to access underlying column-major float array)
> set_uniform_mat4("u_view", view.e);
> set_uniform_mat4("u_view_proj", view_proj.e);
> // (use vec.e to access underlying float array)
> set_uniform_vec4("u_cam_pos_world" cam_pos.e);
> ```

## Example Problem
Let's take a simple example. All matrices are stored in column-major order, as in OpenGL/GLSL/glm.

Say you have a game object `obj` with a transform matrix `mat`. You have another transform matrix, `rot`, describing a rotation. You want to apply `rot` to `mat` such that `obj` has been rotated by `rot`. At this point, any reader with a cursory understanding of graphics programming will likely say something along the lines of "that's so simple, this is a fundamental operation, I could do this in my sleep, why am I reading this, etc, etc".

Such a reader might suggest the following operation:
> ```cpp
> mat = mat * rot
> ```

And they would be correct.

A different reader, with a similar background but different interpretation of the problem, could suggest a slightly different operation:
> ```cpp
> mat = rot * mat
> ```

And they would **also** be correct.

Clearly, the only difference between the two is the order in which the matrices are multiplied. Everyone knows that matrix multiplication is non-commutative, but let's examine what this means (and the restrictions it **should** impose on our math) in a 3D game environment context. In doing so, we will arrive at a fundamental understanding of the difference between the two operations proposed above, along with a more general understanding of the problem and how HATS helps solve it.

## Spaces
The problem description above is incomplete. Experienced readers probably made automatic assumptions about what the given matrices represent, perhaps without even realizing it. The goal of this section is to show **what** those assumptions might be, **why** they are dangerous, and **how** we can write code differently to avoid making any such assumptions at all.

The game object `obj` is some thing that exists in 3D space. Its transformation matrix `mat` describes its position relative to some origin point and its rotation relative to some basis coordinate frame. In general, `mat` describes the local space of `obj` relative to some other space. The obvious question is "relative to what space"? I haven't told you what space, which is the general issue and why the problem doesn't make any sense without additional context.

For this example, the best assumption to make about `mat` is that it describes `obj`'s **local** space relative to the game's **world** space. Let's run with this assumption for now.

Similarly, I have not given the spatial context of `rot`. This is where the question is truly ambiguous; without any context for `rot`, and regardless of the spatial context of `mat`, both `mat * rot` and `rot * mat` are mathematically meaningful operations. They perform different rotations, but both make sense and could be desirable.

In our case, where `mat` transforms from **local** to **world**, the operations have the following effects:
 - `mat * rot` performs the rotation in **local** space (relative to `obj`'s origin)
 - `rot * mat` performs the rotation in **world** space (relative to the world's origin)
 
Clearly, both of these are valid operations and should be allowed. But, it would be nice if `rot`:
1. told us what space the rotation is meant to be applied in
2. forced us to use it how it's meant to be used

**These are the two problems HATS solves.**

## Example Problem Using HATS
*(`tmat` represents a transformation matrix; see [matrix types](#hats-matrix-types) for details)*
Let's write the example above using HATS types and see what difference it makes:
> ```cpp
> using namespace hats;
> 
> //
> // entity.h
> //
> struct entity
> {
>     // this matrix explicitly transforms from local (object) space to world space
>     tmat<space::OBJECT, space::WORLD> mat;
>     // ...
> };
> 
> //
> // main.cpp
> //
> 
> // will compile
> void rotate_entity_local(entity* const e, const tmat<space::OBJECT, space::OBJECT>& rot)
> {
>     // mat = mat * rot
>     // Makes sense because rot describes a rotation in
>     // local (object) space and is being applied in local (object) space.
>     e->mat *= rot;
> }
> void rotate_entity_world(entity* const e, const tmat<space::WORLD, space::WORLD>& rot)
> {
>     // mat = rot * mat
>     // Makes  sense because rot describes a rotation in
>     // world space and is being applied in world space.
>     e->mat = rot * e->mat;
> }
> 
> // won't compile
> void BAD_rotate_entity(entity* const e, const tmat<space::OBJECT, space::OBJECT>& rot)
> {
>     // mat = rot * mat
>     // Doesn't make sense because rot describes a rotation in
>     // local (object) space but is attempting to be applied in world space.
>     e->mat = rot * e->mat;
> }
> void BAD_rotate_entity(entity* const e, const tmat<space::WORLD, space::WORLD>& rot)
> {
>     // mat = mat * rot
>     // Doesn't make sense because rot describes a rotation in
>     // world space but is attempting to be applied in local (object) space.
>     e->mat *= rot;
> }
> ```

The above code shows several important things:

- How using HATS types encodes the assumptions about mathematical operations into the code itself, which is good for readability and forces you to think about what you're doing as you're trying to do it.
- Some examples of the compiler catching incorrect math.
- How "pure" operations, such as the rotation, are encoded in HATS types. These operations can be thought of as changing the meaning of a space directly, rather than transforming between spaces. In this case, the `FROM` and `TO` space of the matrix are the same, showing that the rotation is applied to a space itself. In `rotate_entity_local`, `rot` can be thought of as rotating `OBJECT` space directly. In `rotate_entity_world`, `rot` can be thought of as adding an additional rotation in the conversion between `OBJECT` and `WORLD` space.

## HATS Matrix Types
Matrices in HATS are aware of their spatial context. Namely, every matrix knows the space it transforms `FROM` and the space it transforms `TO`. In the example above, `mat` transforms **from** `OBJECT` **to** `WORLD` space. How these spaces are encoded is the key to their usefulness.

Spaces are used to ensure mathematical validity on all operations involving HATS types. Spaces are enumerated and stored in HATS types as template parameters (see [limitations](#limitations)). This means two things:

 1. The space information uses 0 bytes in the object itself, because it is part of the type
 2. The **compiler** ensures validity of operations by only generating template function specializations for meaningful type combinations

These features are good because:

 1. There is truly no runtime overhead for the niceties HATS provides, in terms of space or time
 2. If your math doesn't make sense, your code won't even compile

There are 3 types of matrices in HATS: `tmat`, `pmat`, and `mat`. They all represent the same underlying structure (a 4x4 matrix of float32's) but have some nice features for different use cases.

### `tmat`
Transformation MATrices represent the standard translate/rotate/scale matrix used by most game objects. It's upper-left 3x3 submatrix and the upper 3 entries of its rightmost column give the orthogonal basis vectors of its space and position of its origin, respectively, relative to the space it transforms `TO`. A `tmat` looks like this:
  > | `i ` | `j ` | `k ` | `t ` |
  > |--|--|--|--|
  > | `x0` | `y0` | `z0` | `t0` |
  > | `x1` | `y1` | `z1` | `t1` |
  > | `x2` | `y2` | `z2` | `t2` |
  > | `0 ` | `0 ` | `0 ` | `1 ` |

  For a matrix transforming **from** `OBJECT` **to** `WORLD` space:
  > - `i`, `j`, and `k` are `WORLD`-space vectors describing the `x`, `y`, and `z` basis vectors of `OBJECT` space relative to `WORLD` space
  > - `t` is a `WORLD`-space point describing the translation of `OBJECT` space relative to `WORLD` space

  An important property of this layout of is that any operation between two `tmat`s always produces another `tmat`. Upon construction, the basis of a `tmat` is automatically renormalized using Gram-Schmidt if it is not within acceptable error bounds. The advantage of this is that the user doesn't have to worry about compounding floating-point rounding error. The disadvantages are:
  > - `tmat`s do not support shear transformations: their basis vectors must be orthogonal
  > - there is some imposed runtime overhead to renormalize (but there would be anyway; ignoring shears, you always want to normalize your basis, HATS just does it for you)
  
### `pmat`
Projection MATrices represent perspective projections. It converts from some space (usually `CAMERA`) to `CLIP` space. A perspective projection is described by four quantities: 
  > 1. `fov = y-axis fov`
  > 2. `ar = aspect ratio`
  > 3. `n = distance to near plane`
  > 4. `f = distance to far plane`

  The following values are derived from these quantities:
  >  - `g = 1 / tan(.5 * fov)`
  > - `x = g / ar`
  > - `y = g`
  > - `a = f / (n - f)`
  > - `b = -f * n / (f - n)`
 
  An additional value `w` is used to swap handedness as necessary. For OpenGL, `w = -1`. A `pmat` looks like this:
  > | `i ` | `j ` | `k ` | `t ` |
  > |--|--|--|--|
  > | `x` | `0` | `0` | `0` |
  > | `0` | `y` | `0` | `0` |
  > | `0` | `0` | `a` | `b` |
  > | `0` | `0` | `w` | `0` |

  You can use the `pmat_util::projection` function to create a `pmat` given the four quantities described above.

### `mat`
MATrices represent arbitrary 4x4 matrices with no format restrictions. These should be used only when necessary, as the other two types provide nice interfaces for their specific use cases. `mat`s are typically used only to store the result of a `tmat` times a `pmat`, for example, a camera's view-projection matrix:

  > ```cpp
  > tmat<space::WORLD, space::CAMERA> view;
  > pmat<space::CAMERA, space::CLIP> proj;
  > const mat<space::WORLD, space:CLIP>& vp = proj * view;
  > ```

## HATS Vectors
Transformation matrices that are compiler validated to be mathematically correct are great, but pretty pointless if you have nothing to transform. Similar to matrices, HATS provides various [vector types](#hats-vector-types) with type-system-encoded spatial context. Unlike matrices, which transform between spaces, vectors exist in a single space (so only have one template parameter).

Vectors in a space `S` can be transformed by matrices transforming **from** `S` **to** any other space. More formally, `mat<FROM, TO> * vec<FROM>` is valid for any `FROM` and any `TO`. Conceptually, such an operation takes a vector in space `FROM` and returns the representation of that vector in space `TO`.

For example, let's say we want to transform the origin point of an entity in **local** space into a point in **camera** space:
>```cpp
>// using the definition of the entity type above
> const tmat<space::OBJECT, space::WORLD>& entity2world = entity->mat;
> // assuming we have some other type representing a camera
> const tmat<space::WORLD, space::CAMERA>& view = cam->view;
> const point<space::WORLD>& entity_origin_world = entity2world.get_t();
> const point<space::CAMERA>& entity_origin_camera = entity_origin_world.transform_copy(view);
>```
Note the following relationships:

- The `SPACE` of `entity_origin_world` matches the `TO` space of `entity2world`
- The `FROM` space of `view` matches the `SPACE` of `entity_origin_world` (meaning `entity_origin_world` can be transformed by `view`)
- The `SPACE` of `entity_origin_camera` matches the `TO` space of `view` (meaning that transforming `entity_origin_world` by `view` produces a vector in `view`'s `TO` space)

## HATS Vector Types
Similar to matrices, there are multiple types of vectors provided for convenience.
### `vec`
Represents a 3D vector, stored internally as a homogenous 4D vector (i.e., w-coordinate is 0).

### `direction`
Represents a 3D unit vector, stored internally as a homogenous 4D vector (i.e., w-coordinate is 0). A `direction` will remain normalized after any operation.

### `point`
Represents a 3D point, stored internally as a homogenous 4D vector (i.e., w-coordinate is 1). 

## Limitations
Spaces are encoded as compile-time constant integers. Given that, you cannot create a dynamic entity system where each entity has its own space ID assigned at runtime. 
- This can be confusing when working with two entities at the same time, both of which have a transformation from "**object**" to **world** space, because the **object** spaces are not necessarily the same. In these cases, HATS types can and should still be used, but additional care must be taken when dealing with the two distinct spaces both encoded as `space::OBJECT` because the compiler doesn't understand that they are different spaces.
- This means that transforming HATS types between different spaces requires a copy. For example, a `vec<space::OBJECT>`, when multiplied by a matrix `mat<space::OBJECT, space::WORLD>`, produces a `vec<space::WORLD>`. Clearly, a `vec<space::WORLD>` cannot be assigned back to the original vector (a `vec<space::OBJECT>`) because they are different types. Because of this, the transformation must be applied in the following way:
  > ```cpp
  > const vec<space::WORLD>& world_vec = object_vec.transform_copy(mat);
  > // OR
  > const vec<space::WORLD>& world_vec = mat * object_vec;
  > // BUT NOT (won't compile)
  > object_vec = mat * object_vec;
  > ``` 
  The `transform_copy` function does the transformation math under the hood and exposes the result to the user as a properly-typed vector. The same pattern applies to transforming, transposing, and inverting matrices.

## Future Work
In no particular order:

- Optimized math routines
- Refactor core types to not use inheritance
	- It's never really useful to store a collection of generic `mat` pointers, for example
	- Virtual function calls are costly
- Add quaternions
- Additional spaces/aliases for better code readability
