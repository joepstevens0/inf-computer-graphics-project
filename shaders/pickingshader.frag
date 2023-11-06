#version 330 core

uniform int objectid;


void main()
{
    gl_FragColor = vec4(objectid/255.0,0,0,1.0f);
}

