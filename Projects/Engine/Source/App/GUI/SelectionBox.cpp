#include <App/GUI/SelectionBox.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/Tree/PVInstance.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <OGRE/Ogre.h>

namespace RNR
{
    void SelectionBox::setAdorneeEvent(Instance* instance)
    {
        Instance* old_adornee = getAdornee();
        if(instance)
        {
            PartInstance* adornee_basepart = dynamic_cast<PartInstance*>(instance);
            if(adornee_basepart)
            {
                getNode()->setScale(adornee_basepart->getSize());
                getNode()->setPosition(adornee_basepart->getPosition());
                getNode()->setVisible(true);
            }
            ModelInstance* adornee_model = dynamic_cast<ModelInstance*>(instance);
            if(adornee_model)
            {
                getNode()->setScale(adornee_model->getBoundingBox().getSize());
                getNode()->setPosition(adornee_model->getBoundingBox().getMinimum());
                getNode()->setVisible(true);
            }
        }
        else
            getNode()->setVisible(false);
    }

    SelectionBox::SelectionBox() : InstanceAdornment()
    {
        setUseIdentityProjection(true);
        setUseIdentityView(true);
        setQueryFlags(0);

        begin(m_material);

        position(0.5,-0.5,1.0);normal(0.408248,-0.816497,0.408248);textureCoord(1,0);
        position(-0.5,-0.5,0.0);normal(-0.408248,-0.816497,-0.408248);textureCoord(0,1);
        position(0.5,-0.5,0.0);normal(0.666667,-0.333333,-0.666667);textureCoord(1,1);
        position(-0.5,-0.5,1.0);normal(-0.666667,-0.333333,0.666667);textureCoord(0,0);
        position(0.5,0.5,1.0);normal(0.666667,0.333333,0.666667);textureCoord(1,0);
        position(-0.5,-0.5,1.0);normal(-0.666667,-0.333333,0.666667);textureCoord(0,1);
        position(0.5,-0.5,1.0);normal(0.408248,-0.816497,0.408248);textureCoord(1,1);
        position(-0.5,0.5,1.0);normal(-0.408248,0.816497,0.408248);textureCoord(0,0);
        position(-0.5,0.5,0.0);normal(-0.666667,0.333333,-0.666667);textureCoord(0,1);
        position(-0.5,-0.5,0.0);normal(-0.408248,-0.816497,-0.408248);textureCoord(1,1);
        position(-0.5,-0.5,1.0);normal(-0.666667,-0.333333,0.666667);textureCoord(1,0);
        position(0.5,-0.5,0.0);normal(0.666667,-0.333333,-0.666667);textureCoord(0,1);
        position(0.5,0.5,0.0);normal(0.408248,0.816497,-0.408248);textureCoord(1,1);
        position(0.5,-0.5,1.0);normal(0.408248,-0.816497,0.408248);textureCoord(0,0);
        position(0.5,-0.5,0.0);normal(0.666667,-0.333333,-0.666667);textureCoord(1,0);
        position(-0.5,-0.5,0.0);normal(-0.408248,-0.816497,-0.408248);textureCoord(0,0);
        position(-0.5,0.5,1.0);normal(-0.408248,0.816497,0.408248);textureCoord(1,0);
        position(0.5,0.5,0.0);normal(0.408248,0.816497,-0.408248);textureCoord(0,1);
        position(-0.5,0.5,0.0);normal(-0.666667,0.333333,-0.666667);textureCoord(1,1);
        position(0.5,0.5,1.0);normal(0.666667,0.333333,0.666667);textureCoord(0,0);

        end();

        setNode(world->getOgreSceneManager()->getRootSceneNode()->createChildSceneNode());
        getNode()->attachObject(this);
    }

    SelectionBox::~SelectionBox()
    {
        getNode()->removeAndDestroyChild(getNode());
    }
}
