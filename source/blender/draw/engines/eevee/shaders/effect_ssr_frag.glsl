
vec3 generate_ray(vec3 V, vec3 N)
{
	return -reflect(-V, N);
}

#ifdef STEP_RAYTRACE

uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specroughBuffer;

layout(location = 0) out vec4 hitData;
layout(location = 1) out vec4 pdfData;

void main()
{
	ivec2 fullres_texel = ivec2(gl_FragCoord.xy) * 2;
	ivec2 halfres_texel = ivec2(gl_FragCoord.xy);
	float depth = texelFetch(depthBuffer, halfres_texel, 0).r;

	/* Early discard */
	if (depth == 1.0)
		discard;

	vec2 uvs = gl_FragCoord.xy / vec2(textureSize(depthBuffer, 0));

	/* Using view space */
	vec3 viewPosition = get_view_space_from_depth(uvs, depth);
	vec3 V = viewCameraVec;
	vec3 N = normal_decode(texelFetch(normalBuffer, fullres_texel, 0).rg, V);

	/* Retrieve pixel data */
	vec4 speccol_roughness = texelFetch(specroughBuffer, fullres_texel, 0).rgba;
	float roughness = speccol_roughness.a;
	float roughnessSquared = roughness * roughness;

	/* Generate Ray */
	vec3 R = generate_ray(V, N);

	/* Search for the planar reflection affecting this pixel */
	/* If no planar is found, fallback to screen space */

	/* Raycast over planar reflection */
	/* Potentially lots of time waisted here for pixels
	 * that does not have planar reflections. TODO Profile it. */
	/* TODO: Idea, rasterize boxes around planar
	 * reflection volumes (frontface culling to avoid overdraw)
	 * and do the raycasting, discard pixel that are not in influence.
	 * Add stencil test to discard the main SSR.
	 * Cons: - Potentially raytrace multiple times
	 *         if Planar Influence overlaps. */
	//float hit_dist = raycast(depthBuffer, W, R);

	/* Raycast over screen */
	float hit = raycast(depthBuffer, viewPosition, R);

	hitData = vec4(hit, hit, hit, 1.0);
	pdfData = vec4(0.5);
}

#else /* STEP_RESOLVE */

uniform sampler2D depthBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D specroughBuffer;

uniform sampler2D hitBuffer;
uniform sampler2D pdfBuffer;

uniform int probe_count;

uniform mat4 ViewProjectionMatrix;

out vec4 fragColor;

void fallback_cubemap(vec3 N, vec3 V, vec3 W, float roughness, float roughnessSquared, inout vec4 spec_accum)
{
	/* Specular probes */
	vec3 spec_dir = get_specular_dominant_dir(N, V, roughnessSquared);

	/* Starts at 1 because 0 is world probe */
	for (int i = 1; i < MAX_PROBE && i < probe_count && spec_accum.a < 0.999; ++i) {
		CubeData cd = probes_data[i];

		float fade = probe_attenuation_cube(cd, W);

		if (fade > 0.0) {
			vec3 spec = probe_evaluate_cube(float(i), cd, W, spec_dir, roughness);
			accumulate_light(spec, fade, spec_accum);
		}
	}

	/* World Specular */
	if (spec_accum.a < 0.999) {
		vec3 spec = probe_evaluate_world_spec(spec_dir, roughness);
		accumulate_light(spec, 1.0, spec_accum);
	}
}

void main()
{
	ivec2 halfres_texel = ivec2(gl_FragCoord.xy / 2.0);
	ivec2 fullres_texel = ivec2(gl_FragCoord.xy);
	vec2 uvs = gl_FragCoord.xy / vec2(textureSize(depthBuffer, 0));

	float depth = textureLod(depthBuffer, uvs, 0.0).r;

	/* Early discard */
	if (depth == 1.0)
		discard;

	/* Using world space */
	vec3 worldPosition = get_world_space_from_depth(uvs, depth);
	vec3 V = cameraVec;
	vec3 N = mat3(ViewMatrixInverse) * normal_decode(texelFetch(normalBuffer, fullres_texel, 0).rg, V);
	vec4 speccol_roughness = texelFetch(specroughBuffer, fullres_texel, 0).rgba;
	float roughness = speccol_roughness.a;
	float roughnessSquared = roughness * roughness;

	vec4 spec_accum = vec4(0.0);

	/* Resolve SSR and compute contribution */

	/* We generate the same rays that has been genearted in the raycast step.
	 * But we add this ray from our resolve pixel position, increassing accuracy. */
	vec3 R = generate_ray(-V, N);
	float ray_length = texelFetch(hitBuffer, halfres_texel, 0).r;

	if (ray_length != -1.0) {
		vec3 hit_pos = worldPosition + R * ray_length;
		vec4 hit_co = ViewProjectionMatrix * vec4(hit_pos, 1.0);
		spec_accum.xy = (hit_co.xy / hit_co.w) * 0.5 + 0.5;
		spec_accum.xyz = vec3(mod(dot(floor(hit_pos.xyz * 5.0), vec3(1.0)), 2.0));
		spec_accum.a = 1.0;
	}

	/* If SSR contribution is not 1.0, blend with cubemaps */
	if (spec_accum.a < 1.0) {
		fallback_cubemap(N, V, worldPosition, roughness, roughnessSquared, spec_accum);
	}

	fragColor = vec4(spec_accum.rgb * speccol_roughness.rgb, 1.0);
	// fragColor = vec4(texelFetch(hitBuffer, halfres_texel, 0).rgb, 1.0);
	// fragColor = vec4(R, 1.0);
}

#endif