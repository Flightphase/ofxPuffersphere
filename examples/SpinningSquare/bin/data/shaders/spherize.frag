#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect tex;    // old (possibly pre-recorded) texture
uniform vec2 imageSize;	// imageSize
uniform float fov;
uniform int projection_method;
uniform float Z;
uniform vec3 lensCorr;

void main()
{

    int method = projection_method;
    float pi = 3.14159265358979;
    float W = imageSize.x;
    float H = imageSize.y;

    float rads = 2.0 * pi / W;

    vec2 pos = gl_TexCoord[0].xy;
    float x = pos.x;
    float y = pos.y;

    float dx = x - W/2.0;
    float dy = y - H/2.0;
    float a = atan( dy,dx );

    float r = sqrt( dx*dx + dy*dy );

    float rho_FOV = ( fov * pi / 180.0 );

    float rho_fov;
    float rho;
    float theta;

    float xe;

    float ye;
    float lat;
    float lon;


    //Inverse  transforms


    //Gnomonic
    if ( method == 0 )
    {
        //Compute rho at fov

        rho_fov = tan( rho_FOV );
        //Scaled radius
        rho = ( r / Z ) * rho_fov;
        theta = atan(rho);

    };

    // Stereographic
    if ( method == 1 )
    {
        rho_fov = tan( rho_FOV / 2.0 );
        rho = ( r / Z ) * rho_fov;
        theta = 2.0 * atan( rho );

    };

    // Orthographic
    if ( method == 2 )

    {
        rho_fov = tan( rho_FOV / 2.0 );
        rho = ( r / Z ) * rho_fov;
        if ( rho < 1.0 )

        {
            theta = asin( rho );
        };

    };

    //Lambert
    if ( method == 3 )
    {
        rho_fov = tan( rho_FOV / 4.0 );
        rho = ( r / Z ) * rho_fov;

        if ( rho < 1.0 )
        {
            theta = asin( rho );

            float f;

            f = theta;
            // f = (f * 1.0801) + (f * f * 0.042) - (f * f * f * 0.1221);
            f = (f * lensCorr.x) + (f * f * lensCorr.y) - (f * f * f * lensCorr.z);
            theta = 2.*f ;

        };


    };

    if ( ( ( method != 2 ) && ( method != 3 ) ) || ( rho < 1.0 ) )
    {

        // Convert back to lat and long


        // Polar coordinates
        lat = theta;
        lon = a - pi / 4.0;

        // Wrap latitude & longditude
        lat = mod( lat + pi, pi ) - pi / 2.0;
        lon = mod( lon + pi, pi * 2.0 ) - pi;

        // Convert to equirectangular coordinates
        xe = -lon/rads;
        ye = lat/rads;

    	gl_FragColor = gl_Color * texture2DRect(tex, vec2( W - (W/2.0 - xe), H/2.0 + ye ));
    }
    else {
        gl_FragColor = vec4(0.0,0.0,0.0,0.0);
//        xe = -1.0;
//        ye = -1.0;
    };

	// gl_FragColor = gl_Color * texture2DRect(tex, samplerPos);
}
