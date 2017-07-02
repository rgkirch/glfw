#include <chrono>
#include "cinder/Easing.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

class BasicApp : public App {
public:
    void setup() override;
    void draw() override;

    static const int NUM_TILES = 1;

    CameraPersp     mCam;
    gl::BatchRef    mSlices[NUM_TILES];
};

void BasicApp::setup()
{
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef	shader = gl::getStockShader( lambert );

    for( int i = 0; i < NUM_TILES; ++i ) {
        float rel = i / (float)NUM_TILES;
        float sliceHeight = 1.0f / NUM_TILES;
        auto slice = geom::Cube().size( 1, sliceHeight, 1 );
        auto trans = geom::Translate( 0, rel, 0 );
        auto color = geom::Constant( geom::COLOR,
                                     Color( CM_HSV, rel, 1, 1 ) );
        mSlices[i] = gl::Batch::create( slice >> trans >> color,
                                        shader );
    }

    mCam.lookAt( vec3( 2, 3, 2 ), vec3( 0, 0.5f, 0 ) );
}

void BasicApp::draw()
{
    gl::clear();
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::setMatrices( mCam );

    const float delay = 0.25f;
    const float rotationTime = 1.5f;
    const float rotationOffset = 0.1f; // seconds
    const float totalTime = delay + rotationTime +
                            NUM_TILES * rotationOffset;

    float time = fmod( getElapsedFrames() / 30.0f, totalTime );

    for( int i = 0; i < NUM_TILES; ++i ) {
        float rotation = 0;
        float startTime = i * rotationOffset;
        float endTime = startTime + rotationTime;
        if( time > startTime && time < endTime ) rotation = ( time - startTime ) / rotationTime;
        float angle = easeInOutQuint( rotation ) * M_PI / 2.0f;

        gl::ScopedModelMatrix scpModelMtx;
//        gl::rotate( angleAxis( angle, vec3( 0, 1, 0 ) ) );
        mSlices[i]->draw();
    }
}

CINDER_APP( BasicApp, RendererGl )

