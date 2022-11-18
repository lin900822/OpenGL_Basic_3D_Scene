#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct LightDirectional{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;
};

struct LightPoint{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;

    float constant;
    float linear;
    float quadratic;
};

struct LightSpot{
    vec3 pos;
    vec3 color;
    vec3 dirToLight;

    float constant;
    float linear;
    float quadratic;

    float CosPhyInner;
    float CosPhyOutter;
};

uniform Material material;

uniform LightDirectional lightDirectional;

uniform LightPoint lightPoint0;
uniform LightPoint lightPoint1;
uniform LightPoint lightPoint2;
uniform LightPoint lightPoint3;
uniform LightPoint lightPoint4;

uniform vec3 cameraPos;

vec3 CalcLightDirectional(LightDirectional light, vec3 uNormal, vec3 dirToCamera){
    // ambient
    vec3 ambient = material.ambient;

    // diffuse 
    float diffIntensity = max(dot(light.dirToLight, uNormal), 0);
    vec3 diffColor = diffIntensity * light.color * ambient * texture(material.diffuse, TexCoord).rgb;

    // specular
    vec3 R = normalize(reflect(-light.dirToLight, uNormal));
    float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess);
    vec3 specColor = specIntensity * light.color * ambient * texture(material.specular, TexCoord).rgb;

    vec3 result = diffColor + specColor;
    return result;
}

vec3 CalcLightPoint(LightPoint light, vec3 uNormal, vec3 dirToCamera){
    // ambient
    vec3 ambient = material.ambient;

    // attenuation 
    float dist = length(light.pos - FragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // diffuse
    float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal), 0) * attenuation;
    vec3 diffColor = diffIntensity * light.color * texture(material.diffuse, TexCoord).rgb;

    // specular
    vec3 R = normalize(reflect(-normalize(light.pos - FragPos), uNormal));
    float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation;
    vec3 specColor = specIntensity * light.color * texture(material.specular, TexCoord).rgb;

    vec3 result = ambient + diffColor + specColor;
    return result;
}

vec3 CalcLightSpot(LightSpot light, vec3 uNormal, vec3 dirToCamera){
    // ambient
    vec3 ambient = material.ambient;

    // attenuation 
    float dist = length(light.pos - FragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    float spotRatio;
    float cosTheta = dot(normalize(FragPos - light.pos), -light.dirToLight);

    if(cosTheta > light.CosPhyInner){
        spotRatio = 1.0f;
    }
    else if(cosTheta > light.CosPhyOutter){
        spotRatio = (cosTheta - light.CosPhyOutter) / (light.CosPhyInner - light.CosPhyOutter);
    }
    else{
        spotRatio = 0.0f;
    }

    // diffuse
    float diffIntensity = max(dot(normalize(light.pos - FragPos), uNormal), 0) * attenuation * spotRatio;
    vec3 diffColor = diffIntensity * light.color/* * texture(material.diffuse, TexCoord).rgb*/;

    // specular
    vec3 R = normalize(reflect(-normalize(light.pos - FragPos), uNormal));
    float specIntensity = pow(max(dot(R, dirToCamera), 0), material.shininess) * attenuation * spotRatio;
    vec3 specColor = specIntensity * light.color /* * texture(material.specular, TexCoord).rgb*/;

    vec3 result = ambient + diffColor + specColor;
    return result;
}

void main()
{
    vec3 finalResult = vec3(0, 0, 0);
    vec3 uNormal = normalize(Normal);
    vec3 dirToCamera = normalize(cameraPos - FragPos);

    //finalResult += CalcLightDirectional(lightDirectional, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightPoint0, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightPoint1, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightPoint2, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightPoint3, uNormal, dirToCamera);
    finalResult += CalcLightPoint(lightPoint4, uNormal, dirToCamera);

    FragColor = vec4(finalResult, 1.0);
}                                               