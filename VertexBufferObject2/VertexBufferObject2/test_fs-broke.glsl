#version 400

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 frag_colour;

in vec3 FragPos;  
in vec3 Normal;
in vec2 TexCoord;  
  
uniform DirLight dirLight;
uniform PointLight1;
uniform PointLight2;

// Texture sampler
uniform sampler2D ourTexture1;

void main() {
// Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    //vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Phase 2: Point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
        vec3 result = CalcPointLight(PointLight1, norm, FragPos, viewDir);   
		 result += CalcPointLight(PointLight2, norm, FragPos, viewDir);  
    // Phase 3: Spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    frag_colour = vec4(result, 1.0);
	//frag_colour = vec4 (result, 1.0) + texture(ourTexture1,TexCoord);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient = light.ambient * vec3(texture(ourTexture1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(ourTexture1, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(ourTexture1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}