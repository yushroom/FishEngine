
#pragma once

template<class T>
constexpr int ClassID();

class GameObject;
template<>
constexpr int ClassID<GameObject>() { return 1; }

class Component;
template<>
constexpr int ClassID<Component>() { return 2; }

class LevelGameManager;
template<>
constexpr int ClassID<LevelGameManager>() { return 3; }

class Transform;
template<>
constexpr int ClassID<Transform>() { return 4; }

class TimeManager;
template<>
constexpr int ClassID<TimeManager>() { return 5; }

class GlobalGameManager;
template<>
constexpr int ClassID<GlobalGameManager>() { return 6; }

class Behaviour;
template<>
constexpr int ClassID<Behaviour>() { return 8; }

class GameManager;
template<>
constexpr int ClassID<GameManager>() { return 9; }

class AudioManager;
template<>
constexpr int ClassID<AudioManager>() { return 11; }

class ParticleAnimator;
template<>
constexpr int ClassID<ParticleAnimator>() { return 12; }

class InputManager;
template<>
constexpr int ClassID<InputManager>() { return 13; }

class EllipsoidParticleEmitter;
template<>
constexpr int ClassID<EllipsoidParticleEmitter>() { return 15; }

class Pipeline;
template<>
constexpr int ClassID<Pipeline>() { return 17; }

class EditorExtension;
template<>
constexpr int ClassID<EditorExtension>() { return 18; }

class Physics2DSettings;
template<>
constexpr int ClassID<Physics2DSettings>() { return 19; }

class Camera;
template<>
constexpr int ClassID<Camera>() { return 20; }

class Material;
template<>
constexpr int ClassID<Material>() { return 21; }

class MeshRenderer;
template<>
constexpr int ClassID<MeshRenderer>() { return 23; }

class Renderer;
template<>
constexpr int ClassID<Renderer>() { return 25; }

class ParticleRenderer;
template<>
constexpr int ClassID<ParticleRenderer>() { return 26; }

class Texture;
template<>
constexpr int ClassID<Texture>() { return 27; }

class Texture2D;
template<>
constexpr int ClassID<Texture2D>() { return 28; }

class SceneSettings;
template<>
constexpr int ClassID<SceneSettings>() { return 29; }

class GraphicsSettings;
template<>
constexpr int ClassID<GraphicsSettings>() { return 30; }

class MeshFilter;
template<>
constexpr int ClassID<MeshFilter>() { return 33; }

class OcclusionPortal;
template<>
constexpr int ClassID<OcclusionPortal>() { return 41; }

class Mesh;
template<>
constexpr int ClassID<Mesh>() { return 43; }

class Skybox;
template<>
constexpr int ClassID<Skybox>() { return 45; }

class QualitySettings;
template<>
constexpr int ClassID<QualitySettings>() { return 47; }

class Shader;
template<>
constexpr int ClassID<Shader>() { return 48; }

class TextAsset;
template<>
constexpr int ClassID<TextAsset>() { return 49; }

class Rigidbody2D;
template<>
constexpr int ClassID<Rigidbody2D>() { return 50; }

class Physics2DManager;
template<>
constexpr int ClassID<Physics2DManager>() { return 51; }

class Collider2D;
template<>
constexpr int ClassID<Collider2D>() { return 53; }

class Rigidbody;
template<>
constexpr int ClassID<Rigidbody>() { return 54; }

class PhysicsManager;
template<>
constexpr int ClassID<PhysicsManager>() { return 55; }

class Collider;
template<>
constexpr int ClassID<Collider>() { return 56; }

class Joint;
template<>
constexpr int ClassID<Joint>() { return 57; }

class CircleCollider2D;
template<>
constexpr int ClassID<CircleCollider2D>() { return 58; }

class HingeJoint;
template<>
constexpr int ClassID<HingeJoint>() { return 59; }

class PolygonCollider2D;
template<>
constexpr int ClassID<PolygonCollider2D>() { return 60; }

class BoxCollider2D;
template<>
constexpr int ClassID<BoxCollider2D>() { return 61; }

class PhysicsMaterial2D;
template<>
constexpr int ClassID<PhysicsMaterial2D>() { return 62; }

class MeshCollider;
template<>
constexpr int ClassID<MeshCollider>() { return 64; }

class BoxCollider;
template<>
constexpr int ClassID<BoxCollider>() { return 65; }

class SpriteCollider2D;
template<>
constexpr int ClassID<SpriteCollider2D>() { return 66; }

class EdgeCollider2D;
template<>
constexpr int ClassID<EdgeCollider2D>() { return 68; }

class ComputeShader;
template<>
constexpr int ClassID<ComputeShader>() { return 72; }

class AnimationClip;
template<>
constexpr int ClassID<AnimationClip>() { return 74; }

class ConstantForce;
template<>
constexpr int ClassID<ConstantForce>() { return 75; }

class WorldParticleCollider;
template<>
constexpr int ClassID<WorldParticleCollider>() { return 76; }

class TagManager;
template<>
constexpr int ClassID<TagManager>() { return 78; }

class AudioListener;
template<>
constexpr int ClassID<AudioListener>() { return 81; }

class AudioSource;
template<>
constexpr int ClassID<AudioSource>() { return 82; }

class AudioClip;
template<>
constexpr int ClassID<AudioClip>() { return 83; }

class RenderTexture;
template<>
constexpr int ClassID<RenderTexture>() { return 84; }

class MeshParticleEmitter;
template<>
constexpr int ClassID<MeshParticleEmitter>() { return 87; }

class ParticleEmitter;
template<>
constexpr int ClassID<ParticleEmitter>() { return 88; }

class Cubemap;
template<>
constexpr int ClassID<Cubemap>() { return 89; }

class Avatar;
template<>
constexpr int ClassID<Avatar>() { return 90; }

class AnimatorController;
template<>
constexpr int ClassID<AnimatorController>() { return 91; }

class GUILayer;
template<>
constexpr int ClassID<GUILayer>() { return 92; }

class RuntimeAnimatorController;
template<>
constexpr int ClassID<RuntimeAnimatorController>() { return 93; }

class ScriptMapper;
template<>
constexpr int ClassID<ScriptMapper>() { return 94; }

class Animator;
template<>
constexpr int ClassID<Animator>() { return 95; }

class TrailRenderer;
template<>
constexpr int ClassID<TrailRenderer>() { return 96; }

class DelayedCallManager;
template<>
constexpr int ClassID<DelayedCallManager>() { return 98; }

class TextMesh;
template<>
constexpr int ClassID<TextMesh>() { return 102; }

class RenderSettings;
template<>
constexpr int ClassID<RenderSettings>() { return 104; }

class Light;
template<>
constexpr int ClassID<Light>() { return 108; }

class CGProgram;
template<>
constexpr int ClassID<CGProgram>() { return 109; }

class BaseAnimationTrack;
template<>
constexpr int ClassID<BaseAnimationTrack>() { return 110; }

class Animation;
template<>
constexpr int ClassID<Animation>() { return 111; }

class MonoBehaviour;
template<>
constexpr int ClassID<MonoBehaviour>() { return 114; }

class MonoScript;
template<>
constexpr int ClassID<MonoScript>() { return 115; }

class MonoManager;
template<>
constexpr int ClassID<MonoManager>() { return 116; }

class Texture3D;
template<>
constexpr int ClassID<Texture3D>() { return 117; }

class NewAnimationTrack;
template<>
constexpr int ClassID<NewAnimationTrack>() { return 118; }

class Projector;
template<>
constexpr int ClassID<Projector>() { return 119; }

class LineRenderer;
template<>
constexpr int ClassID<LineRenderer>() { return 120; }

class Flare;
template<>
constexpr int ClassID<Flare>() { return 121; }

class Halo;
template<>
constexpr int ClassID<Halo>() { return 122; }

class LensFlare;
template<>
constexpr int ClassID<LensFlare>() { return 123; }

class FlareLayer;
template<>
constexpr int ClassID<FlareLayer>() { return 124; }

class HaloLayer;
template<>
constexpr int ClassID<HaloLayer>() { return 125; }

class NavMeshAreas;
template<>
constexpr int ClassID<NavMeshAreas>() { return 126; }

class HaloManager;
template<>
constexpr int ClassID<HaloManager>() { return 127; }

class Font;
template<>
constexpr int ClassID<Font>() { return 128; }

class PlayerSettings;
template<>
constexpr int ClassID<PlayerSettings>() { return 129; }

class NamedObject;
template<>
constexpr int ClassID<NamedObject>() { return 130; }

class GUITexture;
template<>
constexpr int ClassID<GUITexture>() { return 131; }

class GUIText;
template<>
constexpr int ClassID<GUIText>() { return 132; }

class GUIElement;
template<>
constexpr int ClassID<GUIElement>() { return 133; }

class PhysicMaterial;
template<>
constexpr int ClassID<PhysicMaterial>() { return 134; }

class SphereCollider;
template<>
constexpr int ClassID<SphereCollider>() { return 135; }

class CapsuleCollider;
template<>
constexpr int ClassID<CapsuleCollider>() { return 136; }

class SkinnedMeshRenderer;
template<>
constexpr int ClassID<SkinnedMeshRenderer>() { return 137; }

class FixedJoint;
template<>
constexpr int ClassID<FixedJoint>() { return 138; }

class RaycastCollider;
template<>
constexpr int ClassID<RaycastCollider>() { return 140; }

class BuildSettings;
template<>
constexpr int ClassID<BuildSettings>() { return 141; }

class AssetBundle;
template<>
constexpr int ClassID<AssetBundle>() { return 142; }

class CharacterController;
template<>
constexpr int ClassID<CharacterController>() { return 143; }

class CharacterJoint;
template<>
constexpr int ClassID<CharacterJoint>() { return 144; }

class SpringJoint;
template<>
constexpr int ClassID<SpringJoint>() { return 145; }

class WheelCollider;
template<>
constexpr int ClassID<WheelCollider>() { return 146; }

class ResourceManager;
template<>
constexpr int ClassID<ResourceManager>() { return 147; }

class NetworkView;
template<>
constexpr int ClassID<NetworkView>() { return 148; }

class NetworkManager;
template<>
constexpr int ClassID<NetworkManager>() { return 149; }

class PreloadData;
template<>
constexpr int ClassID<PreloadData>() { return 150; }

class MovieTexture;
template<>
constexpr int ClassID<MovieTexture>() { return 152; }

class ConfigurableJoint;
template<>
constexpr int ClassID<ConfigurableJoint>() { return 153; }

class TerrainCollider;
template<>
constexpr int ClassID<TerrainCollider>() { return 154; }

class MasterServerInterface;
template<>
constexpr int ClassID<MasterServerInterface>() { return 155; }

class TerrainData;
template<>
constexpr int ClassID<TerrainData>() { return 156; }

class LightmapSettings;
template<>
constexpr int ClassID<LightmapSettings>() { return 157; }

class WebCamTexture;
template<>
constexpr int ClassID<WebCamTexture>() { return 158; }

class EditorSettings;
template<>
constexpr int ClassID<EditorSettings>() { return 159; }

class InteractiveCloth;
template<>
constexpr int ClassID<InteractiveCloth>() { return 160; }

class ClothRenderer;
template<>
constexpr int ClassID<ClothRenderer>() { return 161; }

class EditorUserSettings;
template<>
constexpr int ClassID<EditorUserSettings>() { return 162; }

class SkinnedCloth;
template<>
constexpr int ClassID<SkinnedCloth>() { return 163; }

class AudioReverbFilter;
template<>
constexpr int ClassID<AudioReverbFilter>() { return 164; }

class AudioHighPassFilter;
template<>
constexpr int ClassID<AudioHighPassFilter>() { return 165; }

class AudioChorusFilter;
template<>
constexpr int ClassID<AudioChorusFilter>() { return 166; }

class AudioReverbZone;
template<>
constexpr int ClassID<AudioReverbZone>() { return 167; }

class AudioEchoFilter;
template<>
constexpr int ClassID<AudioEchoFilter>() { return 168; }

class AudioLowPassFilter;
template<>
constexpr int ClassID<AudioLowPassFilter>() { return 169; }

class AudioDistortionFilter;
template<>
constexpr int ClassID<AudioDistortionFilter>() { return 170; }

class SparseTexture;
template<>
constexpr int ClassID<SparseTexture>() { return 171; }

class AudioBehaviour;
template<>
constexpr int ClassID<AudioBehaviour>() { return 180; }

class AudioFilter;
template<>
constexpr int ClassID<AudioFilter>() { return 181; }

class WindZone;
template<>
constexpr int ClassID<WindZone>() { return 182; }

class Cloth;
template<>
constexpr int ClassID<Cloth>() { return 183; }

class SubstanceArchive;
template<>
constexpr int ClassID<SubstanceArchive>() { return 184; }

class ProceduralMaterial;
template<>
constexpr int ClassID<ProceduralMaterial>() { return 185; }

class ProceduralTexture;
template<>
constexpr int ClassID<ProceduralTexture>() { return 186; }

class OffMeshLink;
template<>
constexpr int ClassID<OffMeshLink>() { return 191; }

class OcclusionArea;
template<>
constexpr int ClassID<OcclusionArea>() { return 192; }

class Tree;
template<>
constexpr int ClassID<Tree>() { return 193; }

class NavMeshObsolete;
template<>
constexpr int ClassID<NavMeshObsolete>() { return 194; }

class NavMeshAgent;
template<>
constexpr int ClassID<NavMeshAgent>() { return 195; }

class NavMeshSettings;
template<>
constexpr int ClassID<NavMeshSettings>() { return 196; }

class LightProbesLegacy;
template<>
constexpr int ClassID<LightProbesLegacy>() { return 197; }

class ParticleSystem;
template<>
constexpr int ClassID<ParticleSystem>() { return 198; }

class ParticleSystemRenderer;
template<>
constexpr int ClassID<ParticleSystemRenderer>() { return 199; }

class ShaderVariantCollection;
template<>
constexpr int ClassID<ShaderVariantCollection>() { return 200; }

class LODGroup;
template<>
constexpr int ClassID<LODGroup>() { return 205; }

class BlendTree;
template<>
constexpr int ClassID<BlendTree>() { return 206; }

class Motion;
template<>
constexpr int ClassID<Motion>() { return 207; }

class NavMeshObstacle;
template<>
constexpr int ClassID<NavMeshObstacle>() { return 208; }

class TerrainInstance;
template<>
constexpr int ClassID<TerrainInstance>() { return 210; }

class SpriteRenderer;
template<>
constexpr int ClassID<SpriteRenderer>() { return 212; }

class Sprite;
template<>
constexpr int ClassID<Sprite>() { return 213; }

class CachedSpriteAtlas;
template<>
constexpr int ClassID<CachedSpriteAtlas>() { return 214; }

class ReflectionProbe;
template<>
constexpr int ClassID<ReflectionProbe>() { return 215; }

class ReflectionProbes;
template<>
constexpr int ClassID<ReflectionProbes>() { return 216; }

class Terrain;
template<>
constexpr int ClassID<Terrain>() { return 218; }

class LightProbeGroup;
template<>
constexpr int ClassID<LightProbeGroup>() { return 220; }

class AnimatorOverrideController;
template<>
constexpr int ClassID<AnimatorOverrideController>() { return 221; }

class CanvasRenderer;
template<>
constexpr int ClassID<CanvasRenderer>() { return 222; }

class Canvas;
template<>
constexpr int ClassID<Canvas>() { return 223; }

class RectTransform;
template<>
constexpr int ClassID<RectTransform>() { return 224; }

class CanvasGroup;
template<>
constexpr int ClassID<CanvasGroup>() { return 225; }

class BillboardAsset;
template<>
constexpr int ClassID<BillboardAsset>() { return 226; }

class BillboardRenderer;
template<>
constexpr int ClassID<BillboardRenderer>() { return 227; }

class SpeedTreeWindAsset;
template<>
constexpr int ClassID<SpeedTreeWindAsset>() { return 228; }

class AnchoredJoint2D;
template<>
constexpr int ClassID<AnchoredJoint2D>() { return 229; }

class Joint2D;
template<>
constexpr int ClassID<Joint2D>() { return 230; }

class SpringJoint2D;
template<>
constexpr int ClassID<SpringJoint2D>() { return 231; }

class DistanceJoint2D;
template<>
constexpr int ClassID<DistanceJoint2D>() { return 232; }

class HingeJoint2D;
template<>
constexpr int ClassID<HingeJoint2D>() { return 233; }

class SliderJoint2D;
template<>
constexpr int ClassID<SliderJoint2D>() { return 234; }

class WheelJoint2D;
template<>
constexpr int ClassID<WheelJoint2D>() { return 235; }

class NavMeshData;
template<>
constexpr int ClassID<NavMeshData>() { return 238; }

class AudioMixer;
template<>
constexpr int ClassID<AudioMixer>() { return 240; }

class AudioMixerController;
template<>
constexpr int ClassID<AudioMixerController>() { return 241; }

class AudioMixerGroupController;
template<>
constexpr int ClassID<AudioMixerGroupController>() { return 243; }

class AudioMixerEffectController;
template<>
constexpr int ClassID<AudioMixerEffectController>() { return 244; }

class AudioMixerSnapshotController;
template<>
constexpr int ClassID<AudioMixerSnapshotController>() { return 245; }

class PhysicsUpdateBehaviour2D;
template<>
constexpr int ClassID<PhysicsUpdateBehaviour2D>() { return 246; }

class ConstantForce2D;
template<>
constexpr int ClassID<ConstantForce2D>() { return 247; }

class Effector2D;
template<>
constexpr int ClassID<Effector2D>() { return 248; }

class AreaEffector2D;
template<>
constexpr int ClassID<AreaEffector2D>() { return 249; }

class PointEffector2D;
template<>
constexpr int ClassID<PointEffector2D>() { return 250; }

class PlatformEffector2D;
template<>
constexpr int ClassID<PlatformEffector2D>() { return 251; }

class SurfaceEffector2D;
template<>
constexpr int ClassID<SurfaceEffector2D>() { return 252; }

class LightProbes;
template<>
constexpr int ClassID<LightProbes>() { return 258; }

class SampleClip;
template<>
constexpr int ClassID<SampleClip>() { return 271; }

class AudioMixerSnapshot;
template<>
constexpr int ClassID<AudioMixerSnapshot>() { return 272; }

class AudioMixerGroup;
template<>
constexpr int ClassID<AudioMixerGroup>() { return 273; }

class AssetBundleManifest;
template<>
constexpr int ClassID<AssetBundleManifest>() { return 290; }

class Prefab;
template<>
constexpr int ClassID<Prefab>() { return 1001; }

class EditorExtensionImpl;
template<>
constexpr int ClassID<EditorExtensionImpl>() { return 1002; }

class AssetImporter;
template<>
constexpr int ClassID<AssetImporter>() { return 1003; }

class AssetDatabase;
template<>
constexpr int ClassID<AssetDatabase>() { return 1004; }

class Mesh3DSImporter;
template<>
constexpr int ClassID<Mesh3DSImporter>() { return 1005; }

class TextureImporter;
template<>
constexpr int ClassID<TextureImporter>() { return 1006; }

class ShaderImporter;
template<>
constexpr int ClassID<ShaderImporter>() { return 1007; }

class ComputeShaderImporter;
template<>
constexpr int ClassID<ComputeShaderImporter>() { return 1008; }

class AvatarMask;
template<>
constexpr int ClassID<AvatarMask>() { return 1011; }

class AudioImporter;
template<>
constexpr int ClassID<AudioImporter>() { return 1020; }

class HierarchyState;
template<>
constexpr int ClassID<HierarchyState>() { return 1026; }

class GUIDSerializer;
template<>
constexpr int ClassID<GUIDSerializer>() { return 1027; }

class AssetMetaData;
template<>
constexpr int ClassID<AssetMetaData>() { return 1028; }

class DefaultAsset;
template<>
constexpr int ClassID<DefaultAsset>() { return 1029; }

class DefaultImporter;
template<>
constexpr int ClassID<DefaultImporter>() { return 1030; }

class TextScriptImporter;
template<>
constexpr int ClassID<TextScriptImporter>() { return 1031; }

class SceneAsset;
template<>
constexpr int ClassID<SceneAsset>() { return 1032; }

class NativeFormatImporter;
template<>
constexpr int ClassID<NativeFormatImporter>() { return 1034; }

class MonoImporter;
template<>
constexpr int ClassID<MonoImporter>() { return 1035; }

class AssetServerCache;
template<>
constexpr int ClassID<AssetServerCache>() { return 1037; }

class LibraryAssetImporter;
template<>
constexpr int ClassID<LibraryAssetImporter>() { return 1038; }

class ModelImporter;
template<>
constexpr int ClassID<ModelImporter>() { return 1040; }

class FBXImporter;
template<>
constexpr int ClassID<FBXImporter>() { return 1041; }

class TrueTypeFontImporter;
template<>
constexpr int ClassID<TrueTypeFontImporter>() { return 1042; }

class MovieImporter;
template<>
constexpr int ClassID<MovieImporter>() { return 1044; }

class EditorBuildSettings;
template<>
constexpr int ClassID<EditorBuildSettings>() { return 1045; }

class DDSImporter;
template<>
constexpr int ClassID<DDSImporter>() { return 1046; }

class InspectorExpandedState;
template<>
constexpr int ClassID<InspectorExpandedState>() { return 1048; }

class AnnotationManager;
template<>
constexpr int ClassID<AnnotationManager>() { return 1049; }

class PluginImporter;
template<>
constexpr int ClassID<PluginImporter>() { return 1050; }

class EditorUserBuildSettings;
template<>
constexpr int ClassID<EditorUserBuildSettings>() { return 1051; }

class PVRImporter;
template<>
constexpr int ClassID<PVRImporter>() { return 1052; }

class ASTCImporter;
template<>
constexpr int ClassID<ASTCImporter>() { return 1053; }

class KTXImporter;
template<>
constexpr int ClassID<KTXImporter>() { return 1054; }

class AnimatorStateTransition;
template<>
constexpr int ClassID<AnimatorStateTransition>() { return 1101; }

class AnimatorState;
template<>
constexpr int ClassID<AnimatorState>() { return 1102; }

class HumanTemplate;
template<>
constexpr int ClassID<HumanTemplate>() { return 1105; }

class AnimatorStateMachine;
template<>
constexpr int ClassID<AnimatorStateMachine>() { return 1107; }

class PreviewAssetType;
template<>
constexpr int ClassID<PreviewAssetType>() { return 1108; }

class AnimatorTransition;
template<>
constexpr int ClassID<AnimatorTransition>() { return 1109; }

class SpeedTreeImporter;
template<>
constexpr int ClassID<SpeedTreeImporter>() { return 1110; }

class AnimatorTransitionBase;
template<>
constexpr int ClassID<AnimatorTransitionBase>() { return 1111; }

class SubstanceImporter;
template<>
constexpr int ClassID<SubstanceImporter>() { return 1112; }

class LightmapParameters;
template<>
constexpr int ClassID<LightmapParameters>() { return 1113; }

class LightmapSnapshot;
template<>
constexpr int ClassID<LightmapSnapshot>() { return 1120; }
