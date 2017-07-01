#include <chrono>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

auto red = Color(1,0,0);
auto green = Color(0,1,0);
auto blue = Color(0,0,1);

class BasicApp : public App {
public:
    void draw() override {
        gl::clear();
        gl::enableDepthRead();
        gl::enableDepthWrite();

        auto turn = getElapsedFrames() * M_PI * 2 / (60.f * 4);

        CameraPersp cam;
        cam.lookAt( vec3( sin(turn) * 4.5, 4.5f, cos(turn) * 4.5f ), vec3( 0, 1, 0 ) );
        gl::setMatrices( cam );

        auto lambert = gl::ShaderDef().lambert().color();
        auto shader = gl::getStockShader( lambert );
        shader->bind();

        int numSpheres = 64;
        float maxAngle = M_PI * 7;
        float spiralRadius = 1;
        float height = 2;
        float anim = getElapsedFrames() / 30.0f;

        for( int s = 0; s < numSpheres; ++s ) {
            float rel = s / (float)numSpheres;
            float angle = rel * maxAngle;
            float y = fabs( cos( rel * M_PI + anim ) ) * height;
            float r = rel * spiralRadius;
            vec3 offset( r * cos( angle ), y / 2, r * sin( angle ) );
            gl::pushModelMatrix();
            gl::translate( offset );
            gl::scale( vec3( 0.05f, y, 0.05f ) );
            gl::color( Color( CM_HSV, rel, 1, 1 ) );
            gl::drawCube( vec3(), vec3( 1 ) );
            gl::popModelMatrix();
        }
    }
};

CINDER_APP( BasicApp, RendererGl )

