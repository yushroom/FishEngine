#ifndef Scene_hpp
#define Scene_hpp

#include "FishEngine.hpp"
#include "Bounds.hpp"
#include <utility>

namespace FishEngine
{
#if 0
    inline Bounds OctreeChildBound(int child, const Bounds& nodeBound, const Vector3 &pMid)
    {
        Vector3 pMin, pMax;
        Vector3 pMin2 = nodeBound.min();
        Vector3 pMax2 = nodeBound.max();
        pMin.x = (child & 4) ? pMid.x : pMin2.x;
        pMax.x = (child & 4) ? pMax2.x : pMid.x;
        pMin.y = (child & 2) ? pMid.y : pMin2.y;
        pMax.y = (child & 2) ? pMax2.y : pMid.y;
        pMin.z = (child & 1) ? pMid.z : pMin2.z;
        pMax.z = (child & 1) ? pMax2.z : pMid.z;
        Bounds childBound;
        childBound.SetMinMax(pMin, pMax);
        return childBound;
    }
    
    template <typename T>
    struct OctNode
    {
        OctNode()
        {
            for (int i = 0; i < 8; ++i)
            {
                children[i] = nullptr;
            }
        }
        ~OctNode()
        {
            for (int i = 0; i < 8; ++i)
            {
                delete children[i];
            }
        }
        
        OctNode * children[8];
        std::vector<T> data;
    };
    
    template<typename T>
    class Octree
    {
    public:
        Octree(const Bounds &bound, const int maxDepth = 16)
        : m_maxDepth(maxDepth), m_bound(bound)
        {
            
        }
        
        void Add(const T& dataItem, const Bounds& dataBound)
        {
            assert(dataBound.Intersects(m_bound));
            addPrivate(&m_root, m_bound, dataItem, dataBound,  Vector3::DistanceSquared(dataBound.min(), dataBound.max()));
        }
        
    private:
        
        void addPrivate(OctNode<T> *node,
                        const Bounds& nodeBound,
                        const T& dataItem,
                        const Bounds& dataBound,
                        float diag2,
                        int depth = 0)
        {
            // Possibly add data item to current octree node
            auto pmin = nodeBound.min();
            auto pmax = nodeBound.max();
            if (depth >= m_maxDepth ||
                Vector3::DistanceSquared(pmin, pmax) < diag2)
            {
                node->data.push_back(dataItem);
                return;
            }
            
            // Otherwise add data item to octree children
            auto pmid = nodeBound.center();
            //Determine which children the item overlaps
            auto pmin2 = dataBound.min();
            auto pmax2 = dataBound.max();
            bool x[2] = { pmin2.x <= pmid.x, pmax2.x > pmid.x };
            bool y[2] = { pmin2.y <= pmid.y, pmax2.y > pmid.y };
            bool z[2] = { pmin2.z <= pmid.z, pmax2.z > pmid.z };
            bool over[8] = {
                x[0] && y[0] && z[0], x[0] && y[0] && z[1],
                x[0] && y[1] && z[0], x[0] && y[1] && z[1],
                x[1] && y[0] && z[0], x[1] && y[0] && z[1],
                x[1] && y[1] && z[0], x[1] && y[1] && z[1]
            };
            for (int child = 0; child < 8; ++child)
            {
                if (!over[child])
                    continue;
                if (!node->children[child])
                    node->children[child] = new OctNode<T>;
                Bounds childBound = OctreeChildBound(child, nodeBound, pmid);
                addPrivate(node->children[child], childBound, dataItem, dataBound, diag2, depth+1);
            }
        }
        
        int m_maxDepth;
        Bounds m_bound;
        OctNode<T> m_root;
    };
    
    typedef Octree<std::weak_ptr<GameObject>> SceneOctree;
    typedef OctNode<std::weak_ptr<GameObject>> SceneOctreeNode;

#endif
    
    class FE_EXPORT Scene
    {
    public:

        Scene() = delete;

        static GameObjectPtr CreateGameObject(const std::string& name);

		static GameObjectPtr CreateCamera();

        static void Init();
        static void Start();
        static void Update();
		static void Clean();
		
        static void RenderShadow(LightPtr const& light);
        static void OnDrawGizmos();

        static GameObjectPtr Find(const std::string& name);

        static void DestroyImmediate(GameObjectPtr g);
        static void DestroyImmediate(ComponentPtr c);

        static void Destroy(GameObjectPtr obj,    const float t = 0.0f);
        static void Destroy(ComponentPtr c,       const float t = 0.0f);
        
        static GameObjectPtr IntersectRay(const Ray& ray);

        static Bounds bounds()
        {
            return m_bounds;
        }
		
		static void Serialize(std::string const & path);

		static std::list<GameObjectPtr> const & GameObjects()
		{
			return m_gameObjects;
		}

		static void AddGameObject(GameObjectPtr const & go)
		{
			m_gameObjects.push_back(go);
		}

    private:
        friend class RenderSystem;
        friend class FishEditor::Inspector;
		//friend class FishEditor::EditorRenderSystem;

        static std::list<GameObjectPtr>   m_gameObjects;
        static std::vector<GameObjectPtr> m_gameObjectsToBeDestroyed;
        static std::vector<ComponentPtr>  m_componentsToBeDestroyed;
        
        static Bounds                   m_bounds;
        //static SceneOctree              m_octree;
        
        static void UpdateBounds();
    };
}

#endif /* Scene_hpp */
