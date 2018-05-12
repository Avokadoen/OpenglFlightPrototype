#version 330 core

out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material snow;
uniform float snowTop;
uniform float snowBottom;

uniform Material stone;
uniform float stoneTop;
uniform float stoneBottom;

uniform Material grass;
uniform float grassTop;
uniform float grassBottom;

uniform Material mud;
uniform float mudTop;
uniform float mudBottom;

uniform Material water;
uniform float waterTop;
uniform float waterBottom;

uniform float maxHeight;
uniform float yOffset;
uniform float yScale;
uniform float highestPoint;
uniform float lerpRange;

/*      LIGHT EMITTER DEFINITIONS       */
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
uniform bool dirSet;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform int pointCount;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_SPOT_LIGHTS 10
uniform SpotLight spotLight [MAX_NR_SPOT_LIGHTS];
uniform int spotCount;

/*      FRAG VARIABLES        */

in vec2 TexCoords;
in vec3 Normal;

uniform vec3 viewPos;
in vec3 FragPos;

#define MAX_NR_DIFFUSE 5
uniform sampler2D texture_diffuse[MAX_NR_DIFFUSE];
uniform int diffuseCount;

#define MAX_NR_SPECULAR 5
uniform sampler2D texture_specular[MAX_NR_SPECULAR];
uniform int specularCount;

/*      FUNCTION DECLARATIONS       */
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
Material applyLerpToMaterial(Material from, Material to, float lerpSum);
float lerp(float v0, float v1, float t);

/*		COLOR OUTPUT VARIABLES 		*/
vec3 diffuseColor;
vec3 specularColor;
Material fragMaterial;

void main()
{
	// apply transform on height
	float fragRelativeHeight = (FragPos.y/(maxHeight * yScale)) - yOffset;

	if(fragRelativeHeight < snowBottom + lerpRange && fragRelativeHeight > snowBottom - lerpRange){
		float lerpStart = snowBottom + lerpRange;
		float lerpEnd = snowBottom - lerpRange;
		float lerpSum = lerp(lerpStart, lerpEnd, fragRelativeHeight-lerpEnd/lerpStart-lerpEnd);
		fragMaterial = applyLerpToMaterial(stone, snow, lerpSum);
	}
	else if(fragRelativeHeight > snowBottom){
		fragMaterial = snow;
	}
	else if(fragRelativeHeight < stoneBottom + lerpRange && fragRelativeHeight > stoneBottom - lerpRange){
		float lerpStart = stoneBottom + lerpRange;
		float lerpEnd = stoneBottom - lerpRange;
		float lerpSum = lerp(lerpStart, lerpEnd, fragRelativeHeight-lerpEnd/lerpStart-lerpEnd);
		fragMaterial = applyLerpToMaterial(grass, stone, lerpSum);
	}
	else if(fragRelativeHeight > stoneBottom){
		fragMaterial = stone;
	}
	else if(fragRelativeHeight > grassBottom + lerpRange && fragRelativeHeight > grassBottom - lerpRange){
		float lerpStart = grassBottom + lerpRange;
		float lerpEnd = grassBottom - lerpRange;
		float lerpSum = lerp(lerpStart, lerpEnd, fragRelativeHeight-lerpEnd/lerpStart-lerpEnd);
		fragMaterial = applyLerpToMaterial(mud, grass, lerpSum);
	}
	else if(fragRelativeHeight > grassBottom){
		fragMaterial = grass;
	}
	else if(fragRelativeHeight < mudBottom + lerpRange && fragRelativeHeight > mudBottom - lerpRange){
		float lerpStart = mudBottom + lerpRange;
		float lerpEnd = mudBottom - lerpRange;
		float lerpSum = lerp(lerpStart, lerpEnd, fragRelativeHeight-lerpEnd/lerpStart-lerpEnd);
		fragMaterial = applyLerpToMaterial(water, mud, lerpSum);
	}
	else if(fragRelativeHeight > mudBottom){
		fragMaterial = mud;
	}
	else{
		fragMaterial = water;
	}

    diffuseColor += fragMaterial.diffuse;
    specularColor += fragMaterial.specular;


    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0.0f);
    // phase 1: directional lighting
    if(dirSet == true){
      result += CalcDirLight(dirLight, norm, viewDir);
    }

    // phase 2: point lights
    for(int i = 0; i < pointCount; i++){
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // phase 3: spot light
    for(int i = 0; i < spotCount; i++){
      result += CalcSpotLight(spotLight[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0f);
}
// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragMaterial.shininess); // mat.shininess
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragMaterial.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragMaterial.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

Material applyLerpToMaterial(Material from, Material to, float lerpSum){
	Material rtr;
	rtr.ambient = from.ambient * lerpSum + to.ambient * (1 - lerpSum);
	rtr.diffuse = from.diffuse * lerpSum + to.diffuse * (1 - lerpSum);
	rtr.specular = from.specular * lerpSum + to.specular * (1 - lerpSum);
	rtr.shininess = from.shininess * lerpSum + to.shininess * (1 - lerpSum);
	return rtr;
}

float lerp(float v0, float v1, float t){
	return (1 - t) * v0 + t * v1;
}
