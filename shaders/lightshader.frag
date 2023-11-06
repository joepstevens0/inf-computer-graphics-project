#version 330 core

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specExp;

    bool hasTex;
    sampler2D texture_diffuse;
};

#define LIGHTOFFK 0.10f
uniform material mat;
uniform bool lightOn;

void main()
{
    if(lightOn)
        gl_FragColor = vec4(mat.diffuse,1.0f);
    else
        gl_FragColor = vec4(LIGHTOFFK* mat.diffuse,1.0f);
}

