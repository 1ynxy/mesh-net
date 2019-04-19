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

uniform vec3 ambientColour;

out vec4 fragColour;

void main() {
	// Sample Textures
	
	vec4 diffColour = texture(diffuseSampler, vTexCoord);
	vec3 emisColour = texture(emissiveSampler, vTexCoord).rgb;
	//vec3 specColour = texture(specularSampler, vTexCoord).rgb;
	//vec3 normalColour = texture(normalSampler, vTexCoord).rgb;
	
	vec3 intensity = vec3(0.0f, 0.0f, 0.0f);
	
	fragColour = vec4(ambientColour, 0.0f) + vec4(emisColour, 0.0f) + (diffColour * vec4(intensity, 1.0f));
}

#endif
