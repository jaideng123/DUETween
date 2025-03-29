# What is DUETween?
DUETween is a framework that allows you to add quick & easy animations to actors in your Unreal Engine games. It provides APIs for both Blueprint and C++ and aims to be flexible, yet memory and runtime efficient.

Current features:
   * Provides an extended library of easing functions (See [easings.net](https://easings.net/)).
   * Lean and efficient using ~1.5mb memory and ticking at <0.5ms for 4000 concurrent tweens.
   * Out of the box functionality for common tweens like location and rotation.
   * Use reflection to conveniently tween UProperties with via field names.
   * Both Blueprint & C++ APIs.

I built DUETween because I fell in love with [DOTween](https://dotween.demigiant.com/) while making Unity games and couldn't find any existing plugins in Unreal that replicated it's power and ease of use.

# How does it work?
## C++ API:

## Blueprint API:

# How to install:
1. Navigate to your Unreal Engine project's "Plugins" directory (create it if it doesn't exist):
`cd path/to/your/project/Plugins`
2. Clone the plugin repository into that folder with: `git clone https://github.com/jaideng123/DUETween.git`
   1. Alternatively, you can just download the zip file and extract it into this folder
   2. If you're more advanced, I would recommend installing this as a submodule in your project see [Adding A SubModule](https://gist.github.com/gitaarik/8735255#adding-a-submodule)
4. Rebuild and re-open your project
5. Load up the test map under `Content/TweenTestMap.umap` and hit play to verify that everything's working
