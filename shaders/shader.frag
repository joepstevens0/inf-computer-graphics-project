#version 330 core

struct material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specExp;

    bool hasTex;
    sampler2D texture_diffuse;
};

struct Light {
    vec3 pos;
    vec3 color;
    bool isSpot;
    vec3 direction;
    float innercutoff;
    float outercutoff;
};

#define MAXLIGHTS 16
uniform int nrlights;
uniform Light lights[MAXLIGHTS];

uniform material mat;
uniform mat4 view;

in vec3 fragPos;
in vec3 norm;
in vec2 texCoord;
#define AMBIENTK 0.10f


void main()
{
    vec3 lighting = vec3(0,0,0);

    vec3 fragToView = normalize(- fragPos);
    for(int i = 0; i < nrlights; ++i)
    {
        // transform light to view pos
        vec3 lightViewPos = vec3(view * vec4(lights[i].pos, 1));
        vec3 fragToLight = normalize(lightViewPos - fragPos);

        // amient light:= ambientlight * ambientmaterial
        vec3 ambient = lights[i].color * mat.ambient * AMBIENTK* mat.diffuse;

        // diffuse:=   diffuselight * diffusematerial * <normal,fragtolight>
        vec3 diffuse =   lights[i].color * mat.diffuse * max(dot(norm, fragToLight), 0.0f);

        // specular:= specularlight * specularmaterial * <lighttofragreflected,fragtoviewer>^specexp
        vec3 reflectDir = reflect(-fragToLight, norm);
        float spec = pow(max(dot(reflectDir, fragToView), 0.0f), mat.specExp);
        vec3 specular = lights[i].color * mat.specular * spec;

        // attenuation
        float d = length(lightViewPos - fragPos);
        float attenuation = min(1/(1 + 0.027f*d + 0.0028f*pow(d, 2)), 1);

        if (lights[i].isSpot){
            vec3 viewlightdir = mat3(view)* lights[i].direction;
            float theta = dot(normalize(lightViewPos - fragPos), normalize(-viewlightdir));
            float epsilon   = cos(radians(lights[i].innercutoff)) - cos(radians(lights[i].outercutoff));
            float intensity = clamp((theta - cos(radians(lights[i].outercutoff))) / epsilon, 0.0, 1.0);
            attenuation *= intensity;
        }

        lighting += attenuation*(ambient + diffuse + specular);
    }


    vec3 minambient = mat.ambient * AMBIENTK * mat.diffuse;
    if (lighting.x < minambient.x)
        lighting.x = minambient.x;
    if (lighting.y < minambient.y)
        lighting.y = minambient.y;
    if (lighting.z < minambient.z)
        lighting.z = minambient.z;
    if (mat.hasTex){
        lighting *= texture2D(mat.texture_diffuse, texCoord).rgb;
    }

    gl_FragColor = vec4(lighting,1.0f);
}

