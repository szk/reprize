#ifndef BULLET_COLLISION_HPP_
#define BULLET_COLLISION_HPP_

#include "Collision.hpp"
#include "util/bullet/btBulletDynamicsCommon.h"
#include "util/bullet/btBulletCollisionCommon.h"

namespace reprize
{
namespace phy
{
typedef res::Command<Collision> CollCmd;

class BulletCollision : public Collision
{
public:
    BulletCollision(void)
        {
        }
    virtual ~BulletCollision(void)
        {
            //cleanup in the reverse order of creation/initialization

            //remove the rigidbodies from the dynamics world and delete them
            for (int i = dyn_wld->getNumCollisionObjects() - 1; i >= 0 ; i--)
            {
		btCollisionObject* obj = dyn_wld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{ delete body->getMotionState(); }
		dyn_wld->removeCollisionObject( obj );
		delete obj;
            }

            //delete collision shapes
            for (int j = 0; j < collisionShapes.size(); j++)
            {
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
            }

            //delete dynamics world
            delete dyn_wld;

            //delete solver
            delete solver;

            //delete broadphase
            delete overlappingPairCache;

            //delete dispatcher
            delete dispatcher;

            delete coll_config;

            // next line is optional:it will be cleared by the destructor when
            // the array goes out of scope
            collisionShapes.clear();
        }

    void init(void)
        {
//            Add(new res::Node("+ShowBoundingVolume",
//                              new CollPropStr(this, &Collision::prop_on_showbv)));
//            Add(new res::Node("-ShowBoundingVolume",
//                              new CollPropStr(this, &Collision::prop_off_showbv)));

            /// collision configuration contains default setup for memory,
            /// collision setup. Advanced users can create their own
            /// configuration.
            coll_config = new btDefaultCollisionConfiguration();

            /// use the default collision dispatcher. For parallel processing
            /// you can use a diffent dispatcher(see Extras/BulletMultiThreaded)
            dispatcher = new btCollisionDispatcher(coll_config);

            /// btDbvtBroadphase is a good general purpose broadphase.
            /// You can also try out btAxis3Sweep.
            overlappingPairCache = new btDbvtBroadphase();

            /// the default constraint solver. For parallel processing
            /// you can use a different solver (see Extras/BulletMultiThreaded)
            solver = new btSequentialImpulseConstraintSolver;

            dyn_wld = new btDiscreteDynamicsWorld(dispatcher,
                                                  overlappingPairCache,
                                                  solver, coll_config);
            dyn_wld->setGravity(btVector3(0, 0, -10));

            /// Ground
            /// create a few basic rigid bodies
            btCollisionShape* grd_shp = new btBoxShape(
                btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

            collisionShapes.push_back(grd_shp);

            btTransform grd_pos;
            grd_pos.setIdentity();
            grd_pos.setOrigin(btVector3(0, 0, -100));

            btScalar mass(0.);

            // rigidbody is dynamic if and only if mass is non zero,
            // otherwise static
            bool isDynamic = (mass != 0.f);

            btVector3 localInertia(0, 0, 0);
            if (isDynamic)
            { grd_shp->calculateLocalInertia(mass, localInertia); }

            // using motionstate is recommended,
            // it provides interpolation capabilities,
            // and only synchronizes 'active' objects
            btDefaultMotionState* m_state = new btDefaultMotionState(grd_pos);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_state,
                                                            grd_shp,
                                                            localInertia);
            btRigidBody* body = new btRigidBody(rbInfo);

            //add the body to the dynamics world
            dyn_wld->addRigidBody(body);

            /// Object
            //create a dynamic rigidbody
            //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
            btCollisionShape* colShape = new btSphereShape(btScalar(1.));
            collisionShapes.push_back(colShape);

            /// Create Dynamic Objects
            btTransform dyn_pos;
            dyn_pos.setIdentity();

            mass = 1.f;

            // rigidbody is dynamic if and only if mass is non zero,
            // otherwise static
            isDynamic = (mass != 0.f);

            btVector3 d_localInertia(0,0,0);
            if (isDynamic)
            { colShape->calculateLocalInertia(mass, d_localInertia); }

            dyn_pos.setOrigin(btVector3(2, 2, 100));

            // using motionstate is recommended, it provides interpolation
            // capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* d_m_state = new btDefaultMotionState(dyn_pos);
            btRigidBody::btRigidBodyConstructionInfo d_rbInfo(mass, d_m_state,
                                                              colShape,
                                                              d_localInertia);
            btRigidBody* d_body = new btRigidBody(d_rbInfo);

            dyn_wld->addRigidBody(d_body);
        }

    virtual void add_table(res::Actor* actor_, const bool base_)
        {
            g_log->printf("----------PHYSICS: %s, %d",
                          actor_->get_name().c_str(), base_);
            if (base_) { base = actor_; }
            else { dynamic = actor_; }
        }

    void tick(void)
        {
            /// Do some simulation
            for (size_t i = 0; i < 100; i++)
            {
//                 dyn_wld->stepSimulation(0.0005f, 10);
                dyn_wld->stepSimulation(1.f/20.f, 10);
//                 dyn_wld->stepSimulation(1.f/60.f, 10);

		//print positions of all objects
		for (int j = dyn_wld->getNumCollisionObjects() - 1; j >= 0 ;j--)
		{
                    btCollisionObject* obj =
                        dyn_wld->getCollisionObjectArray()[j];
                    btRigidBody* body = btRigidBody::upcast(obj);
                    if (body && body->getMotionState())
                    {
                        btTransform pos;
                        body->getMotionState()->getWorldTransform(pos);
//                         g_log->printf("world pos = %f,%f,%f",
//                                       float(pos.getOrigin().getX()),
//                                       float(pos.getOrigin().getY()),
//                                       float(pos.getOrigin().getZ()));
                    }
		}
            }

            btCollisionObject* obj = dyn_wld->getCollisionObjectArray()[1];
            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform pos;
            body->getMotionState()->getWorldTransform(pos);
            if (dynamic != NULL)
            {
                dynamic->set_rel_pos(Vec3(float(pos.getOrigin().getX()),
                                          float(pos.getOrigin().getY()),
                                          float(pos.getOrigin().getZ())));
//                 g_log->printf("world pos = %f,%f,%f",
//                               float(pos.getOrigin().getX()),
//                               float(pos.getOrigin().getY()),
//                               float(pos.getOrigin().getZ()));
            }
        }

    void finish(void)
        {
        }

protected:
    btDiscreteDynamicsWorld* dyn_wld;
    // keep track of the shapes, we release memory at exit. make sure to
    // re-use collision shapes among rigid bodies whenever possible!
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    btSequentialImpulseConstraintSolver* solver;
    btBroadphaseInterface* overlappingPairCache;
    btCollisionDispatcher* dispatcher;
    btDefaultCollisionConfiguration* coll_config;
};
}
}

#endif
