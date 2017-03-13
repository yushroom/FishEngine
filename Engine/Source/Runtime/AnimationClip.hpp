#include "FishEngine.hpp"
#include "Object.hpp"

namespace FishEngine
{
	class Motion : public Object
	{

	};

	// Determines how time is treated outside of the keyframed range of an AnimationClip or AnimationCurve.
	enum class WrapMode
	{
		Once,           // When time reaches the end of the animation clip, the clip will automatically stop playing and time will be reset to beginning of the clip.
		Loop,           // When time reaches the end of the animation clip, time will continue at the beginning.
		PingPong,       // When time reaches the end of the animation clip, time will ping pong back between beginning and end.
		Default,        // Reads the default repeat mode set higher up.
		ClampForever,   // Plays back the animation. When it reaches the end, it will keep playing the last frame and never stop playing.
	};

	// Used by Animation.Play function.
	enum class AnimationBlendMode
	{
		Blend,          // Animations will be blended.
		Additive,       // Animations will be added.
	};

	// The AnimationState gives full control over animation blending.
	class AnimationState
	{
		// Which blend mode should be used?
		AnimationBlendMode blendMode;

		// The clip that is being played by this animation state.
		AnimationClip clip;

		// Enables / disables the animation.
		bool enabled;

		// The length of the animation clip in seconds.
		float length;

		// The name of the animation.
		std::string name;

		// The normalized playback speed.
		float normalizedSpeed;
		
		// The normalized time of the animation.
		float normalizedTime;

		// The playback speed of the animation. 1 is normal playback speed.
		float speed;

		// The current time of the animation.
		float time;

		// The weight of animation.
		float weight;

		// Wrapping mode of the animation.
		WrapMode wrapMode;
	};

	// Information about clip being played and blended by the Animator.
	struct AnimatorClipInfo
	{
		// Returns the animation clip played by the Animator.
		AnimationClip clip;

		// Returns the blending weight used by the Animator to blend this clip.
		float weight;
	};

	// Information about the current or next state.
	struct AnimatorStateInfo
	{

	};

	// AnimationEvent lets you call a script function similar to SendMessage as part of playing back an animation.
	class AnimationEvent
	{
	public:
		//The animation state that fired this event (Read Only).
		AnimationState animationState;

		// The animator clip info related to this event (Read Only).
		AnimatorClipInfo animatorClipInfo;
	};

	// Stores keyframe based animations.
	// AnimationClip is used by Animation to play back animations.
	class AnimationClip : public Motion
	{
	public:

		// Animation Events for this animation clip.
		std::vector<AnimationClip> events;

		// Frame rate at which keyframes are sampled. (Read Only)
		// This is the frame rate that was used in the animation program you used to create the animation or model.
		float frameRate;

		// Animation length in seconds. (Read Only)
		float length;

		// Sets the default wrap mode used in the animation state.
		WrapMode wrapMode;
	};
}