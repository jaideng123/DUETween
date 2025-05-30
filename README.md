# What is DUETween?
DUETween is a framework that allows you to add quick & easy animations to actors in your Unreal Engine games. It provides APIs for both Blueprint and C++ and aims to be flexible, yet memory and runtime efficient.

![due-tween](https://github.com/user-attachments/assets/054ab0b2-5c5a-4fbb-8981-65696fb796ea)

Current features:
   * Provides an extended library of easing functions (See [easings.net](https://easings.net/)).
   * Lean and efficient using ~1.5mb memory and ticking at <0.5ms for 4000 concurrent tweens.
   * Out of the box functionality for common tweens like location and rotation.
   * Use reflection to conveniently tween UProperties with via field names.
   * Both Blueprint & C++ APIs.

I built DUETween because I fell in love with [DOTween](https://dotween.demigiant.com/) while making Unity games and couldn't find any existing plugins in Unreal that replicated it's power and ease of use.

# How does it work?
## C++ API:
To get you started there are some handy methods to do common tasks like rotation and movement:
``` cpp
// Basic Movement:
DUETween::DueMove(this, TargetLocation, 1.0);
// Basic Rotation:
DUETween::DUERotate(this, TargetRotation, 1.0);
// 2D Movement (for UI elements mainly):
DUETween::DueMove2D(this, Target2DLocation, 1.0);
```
Every tween can be used with any of the [available easing functions](https://github.com/jaideng123/DUETween/blob/main/Source/DUETween/Public/DUEEasingFunctionLibrary.h):
``` cpp
DUETween::DueMove(this, TargetLocation, 1.0, EDueEasingType::Linear);
```
If you have a specific property on a UObject you want to tween you can reference it by name:
``` cpp
DUETween::StartDUETween<float>(this,"MyProperty", 50.0, 1.0);
```
Each tween returns a handle that you can use to control playback as well as modify tween properties:
``` cpp
FActiveDUETweenHandle Handle = DUETween::DueMove(this, TargetLocation, 1.0);
// Stop and cancel tween
Handle.StopTween();
// Pause and resume tween
Handle.PauseTween();
Handle.Resume();
// Updates the step count (chunkiness) of the tween
Handle.SetSteps(10);
// Updates whether or not this tween should yoyo (return to it's starting position)
Handle.SetYoYo(true);
// Updates Number Of Loops this tween will do before completing (-1 = infinite)
Handle.SetLoopCount(-1);
// Binds a completion event
FTweenCompleteCallback& CompletionCallback;
Handle.OnComplete(CompletionCallback);
```
(C++ Only) You can also use Lambdas for custom tween callbacks, here's an example where I use a lambda tween to evaluate a custome curve:
``` cpp
const FVector TargetLocation = BallGripStartPosition.GetValue() + GetActorForwardVector() *
  StartDistance;
const FVector StartPosition = BallGripStartPosition.GetValue();
FTweenUpdateCallback TargetCallback = [TargetLocation,StartPosition,Curve = RunUpCurve.GetRichCurveConst(), this](
  const FValueContainer& UpdatedValue)
{
  const FVector CurrentLocation = FMath::Lerp(StartPosition, TargetLocation,
                                              Curve->Eval(UpdatedValue.GetSubtype<float>()));
  this->SetActorLocation(CurrentLocation);
};
DUETween::StartDUETween<float>(this, TargetCallback, 0.0, 1.0, RunUpTimeMS);
```
## Blueprint API:
<iframe src="https://blueprintue.com/render/zc41f01p/" scrolling="no" allowfullscreen></iframe>

# Version Compatibility:
DUETween was built primarily for **Unreal Engine 5** and has been tested with all versions released between 5.0 and 5.5.

There's nothing that would prevent DUETween from working in UE 4 other than some minor code changes.
Feel free to make a fork of your own or open an issue if you require more assistance.

# How to install:
1. Navigate to your Unreal Engine project's "Plugins" directory (create it if it doesn't exist):
`cd path/to/your/project/Plugins`
2. Clone the plugin repository into that folder with: `git clone https://github.com/jaideng123/DUETween.git`
   1. Alternatively, you can just download the zip file and extract it into this folder
   2. If you're more advanced, I would recommend installing this as a submodule in your project see [Adding A SubModule](https://gist.github.com/gitaarik/8735255#adding-a-submodule)
4. Rebuild and re-open your project
5. Enable the Plugin by go to Edit > Plugins, Search for `DUETween` and check the box.
6. Restart the Unreal Engine editor to apply the plugin.
7. Load up the test map under `Content/TweenTestMap.umap` and hit play to verify that everything's working

# Misc
## Tuning your Tween Pool Settings
For performance and memory efficiency. Active tweens in DUETween are lumped together into a pre-allocated pool that initializes at a size of about 100. For ease of use, this pool will automatically grow by about 100 as you use more concurrent tweens. Eventually it will max out at 10000 concurrent tweens and start throwing error logs at you.

If you have strict memory requirements (or just like tinkering for perf) you can adjust these in your project settings under `Engine - Due Tween Settings`

![image](https://github.com/user-attachments/assets/c9f560ec-85b7-4333-92e6-9e6ba3a776c3)
