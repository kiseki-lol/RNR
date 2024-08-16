//---------------------------------------------------------------------------
//These materials/shaders are part of the NEW InstanceManager implementation
//Written by Matias N. Goldberg ("dark_sylinc")
//---------------------------------------------------------------------------

uniform vec4	lightPosition;
uniform vec3	cameraPosition;
uniform vec3 	lightAmbient;
uniform vec3	lightDiffuse;
uniform vec3	lightSpecular;
uniform vec4	lightAttenuation;
uniform float	lightGloss;

#if DEPTH_SHADOWRECEIVER
uniform float invShadowMapSize;
uniform sampler2DShadow shadowMap;

//declare external function
//vec4 calcDepthShadow(in vec4 inColour, in float lum);
float calcDepthShadow(sampler2DShadow shadowMap, vec4 uv, float invShadowMapSize);
#endif

varying vec2 _uv0;
varying vec3 oNormal;
varying vec3 oVPos;
#if DEPTH_SHADOWRECEIVER
    varying vec4 oLightSpacePos;
#endif

varying vec4 partColor;

//---------------------------------------------
//Main Pixel Shader
//---------------------------------------------
void main(void)
{
    float fShadow = 1.0;
#if DEPTH_SHADOWRECEIVER
    fShadow = calcDepthShadow( shadowMap, oLightSpacePos, invShadowMapSize );
#endif

    vec4 baseColour = partColor;

    //Blinn-Phong lighting
    vec3 normal	= normalize( oNormal );
    vec3 lightDir		= lightPosition.xyz - oVPos * lightPosition.w;
    vec3 eyeDir			= normalize( cameraPosition - oVPos );

    float fLength	= length( lightDir );
    lightDir			= normalize( lightDir );

    float NdotL	= max( 0.0, dot( normal, lightDir ) );
    vec3 halfVector		= normalize(lightDir + eyeDir);
    float HdotN	= max( 0.0, dot( halfVector, normal ) );
    
    vec3 ambient  = lightAmbient;
    vec3 diffuse  = lightDiffuse * NdotL;
    vec3 specular = lightSpecular * pow( HdotN, lightGloss );
    
    vec3 directLighting = (diffuse + specular) * fShadow;
    
    gl_FragColor = vec4( baseColour.xyz * (directLighting + ambient), baseColour.a );
    //gl_FragColor = baseColour;
}