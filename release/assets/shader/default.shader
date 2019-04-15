#ifdef COMP_VERT

layout(location = 0) in vec3 vPositionIn;
layout(location = 1) in vec3 vNormalIn;
layout(location = 2) in vec2 vTexCoordIn;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
	vPosition = vec3(modelMat * vec4(vPositionIn, 1.0f));
	vNormal = normalize(mat3(modelMat) * vNormalIn);
	vTexCoord = vec2(vTexCoordIn.x, 1 - vTexCoordIn.y);
	
	gl_Position = projMat * viewMat * vec4(vPosition, 1.0f);
}

#else

struct LightSource {
	vec4 type;
	
	vec4 transform;
	
	vec4 colour;
};

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform sampler2D diffuseSampler;
uniform sampler2D emissiveSampler;
uniform sampler2D specularSampler;
uniform sampler2D normalSampler;

uniform vec3 ambientColour = { 0.05f, 0.05f, 0.1f };

layout (std140, binding = 1) buffer LightSources {
    LightSource sources[];
} lights;

out vec4 fragColour;

vec3 CalcLight(LightSource light) {
	if (light.type.x == 0) {
		vec3 lDirection = light.transform.xyz;
		
		// Calculate Angle
		
		float angle = max(dot(vNormal, lDirection), 0.0);
		
		// Calculate Result
		
		return ((angle * light.colour.xyz) * light.colour.w);
	}
	else {
		vec3 lPosition = light.transform.xyz;
		vec3 lDirection = normalize(lPosition - vPosition);
		
		// Calculate Distance
		
		float distance = distance(vPosition, lPosition);
		
		float attenuation = clamp(1.0f / (1.0f + 0.1f * distance + 0.01f * distance * distance), 0.0f, 1.0f);
		
		// Calculate Angle
		
		float angle = max(dot(vNormal, lDirection), 0.0);
		
		// Calculate Result
		
		return ((angle * light.colour.xyz) * light.colour.w) * attenuation;
	}
}

void main() {
	// Sample Textures
	
	vec4 diffColour = texture(diffuseSampler, vTexCoord);
	vec3 emisColour = texture(emissiveSampler, vTexCoord).rgb;
	//vec3 specColour = texture(specularSampler, vTexCoord).rgb;
	//vec3 normalColour = texture(normalSampler, vTexCoord).rgb;
	
	vec3 intensity = vec3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < lights.sources.length(); ++i) intensity += CalcLight(lights.sources[i]);
	
	fragColour = vec4(ambientColour, 0.0f) + vec4(emisColour, 0.0f) + (diffColour * vec4(intensity, 1.0f));
}

#endif