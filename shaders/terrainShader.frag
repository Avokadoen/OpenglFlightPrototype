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
//uniform float yOffset;
//uniform float yScale;
//uniform float highestPoint;
//uniform float lerpRange;

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
	float lerpRange = 0.05f;
	float fragRelativeHeight = FragPos.y/maxHeight;


	if(fragRelativeHeight > snowBottom){
		fragMaterial = snow;

		if(fragRelativeHeight < snowBottom + lerpRange){
			float lerpPos = ((fragRelativeHeight - snowBottom) / lerpRange);
			fragMaterial = applyLerpToMaterial(stone, snow, lerpPos);
		}
	}
	else if(fragRelativeHeight > stoneBottom){
		fragMaterial = stone;

		if(fragRelativeHeight < stoneBottom + lerpRange){
			float lerpPos = ((fragRelativeHeight - stoneBottom) / lerpRange);
			fragMaterial = applyLerpToMaterial(grass, stone, lerpPos);
		}
	}
	else if(fragRelativeHeight > grassBottom){
		fragMaterial = grass;

		if(fragRelativeHeight < grassBottom + lerpRange){
			float lerpPos = ((fragRelativeHeight - grassBottom) / lerpRange);
			fragMaterial = applyLerpToMaterial(mud, grass, lerpPos);
		}
	}
	else if(fragRelativeHeight > mudBottom){
		fragMaterial = mud;

		if(fragRelativeHeight < mudBottom + lerpRange){
			float lerpPos = ((fragRelativeHeight - mudBottom) / lerpRange);
			fragMaterial = applyLerpToMaterial(water, mud, lerpPos);
		}
	}
	else{
		fragMaterial = water;
	}

    diffuseColor = fragMaterial.diffuse;
    specularColor = fragMaterial.specular;

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
	rtr.ambient.x = lerp(from.ambient.x, to.ambient.x, lerpSum);
	rtr.ambient.y = lerp(from.ambient.y, to.ambient.y, lerpSum);
	rtr.ambient.z = lerp(from.ambient.z, to.ambient.z, lerpSum);

	rtr.diffuse.x = lerp(from.diffuse.x , to.diffuse.x, lerpSum);
	rtr.diffuse.y = lerp(from.diffuse.y , to.diffuse.y, lerpSum);
	rtr.diffuse.z = lerp(from.diffuse.z , to.diffuse.z, lerpSum);

	rtr.specular.x = lerp(from.specular.x , to.specular.x, lerpSum);
	rtr.specular.y = lerp(from.specular.y , to.specular.y, lerpSum);
	rtr.specular.z = lerp(from.specular.z , to.specular.z, lerpSum);

	rtr.shininess = lerp(from.shininess , to.shininess, lerpSum);
	return rtr;
}

float lerp(float v0, float v1, float t){
	return (1 - t) * v0 + t * v1;
}
