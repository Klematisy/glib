#define p(t, a, b, c, d) ( a + b*cos( 6.28318*(c*t+d) ) )
#define sp(t) p(t,vec3(.26,.76,.77),vec3(1,.3,1),vec3(.8,.4,.7),vec3(0,.12,.54))
#define hue(v) ( .6 + .76 * cos(6.3*(v) + vec4(0,23,21,0) ) )

#define smoothing 0.006
#define TWO_PI    6.28318530718

#define MountainLayerThreecol vec3(26., 65., 74.)/255.
#define MountainLayerFourCol vec3(14., 49., 55.)/255.
#define FieldMid              vec3(94., 121., 62.)/255.

float hash(float p) {
    p = fract(p * .1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

float noise(float x) {
    float i = floor(x);
    float f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    return mix(hash(i), hash(i + 1.0), f);
}

float hash12(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec2 hash22(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx + p3.yz) * p3.zy);
}

vec2 rotate2D(vec2 st, float a) {
    return mat2(cos(a), -sin(a), sin(a), cos(a)) * st;
}

float st(float a, float b, float s) {
    return smoothstep(a - s, a + s, b);
}

float noise(in vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3. - 2. * f);
    return mix(mix(dot(hash22(i + vec2(0, 0)), f - vec2(0, 0)),
            dot(hash22(i + vec2(1, 0)), f - vec2(1, 0)), u.x),
        mix(dot(hash22(i + vec2(0, 1)), f - vec2(0, 1)),
            dot(hash22(i + vec2(1, 1)), f - vec2(1, 1)), u.x), u.y);
}

float s_noise(vec2 p) {
    return noise(p) * 0.5 + 0.5;
}

vec3 getPaintedSkyAndMountains(vec2 g, vec2 r, float time) {
    vec2 uv = (g + g - r) / r.y;
    vec2 sun_pos = vec2(r.x / r.y * 0.35, -0.15);
    vec2 sh = rotate2D(sun_pos, noise(uv + time * .25) * .3);

    vec3 f = vec3(0);
    float sm = 3. / r.y;
    vec2 u, id, lc, t;
    float xd, yd, h;

    u = uv + sh;
    yd = 60.;
    id = vec2((length(u) + .01) * yd, 0);
    xd = floor(id.x) * .09;
    h = (hash12(floor(id.xx)) * .5 + .25) * (time + 10.) * .25;
    t = rotate2D(u, h);
    id.y = atan(t.y, t.x) * xd;
    lc = fract(id);
    id -= lc;

    t = vec2(cos((id.y + .5) / xd) * (id.x + .5) / yd, sin((id.y + .5) / xd) * (id.x + .5) / yd);
    t = rotate2D(t, -h) - sh;

    h = noise(t * vec2(.5, 1) - vec2(time * .2, 0)) * step(-.25, t.y);
    h = smoothstep(.052, .055, h);
    lc += (noise(lc * vec2(1, 4) + id)) * vec2(.7, .2);

    f = mix(sp(sin(length(u) - .1)) * .35,
            mix(sp(sin(length(u) - .1) + (hash12(id) - .5) * .15), vec3(1), h),
            st(abs(lc.x - .5), .4, sm * yd) * st(abs(lc.y - .5), .48, sm * xd));

    float cld = noise(-sh * vec2(.5, 1) - vec2(time * .2, 0));
    cld = 1. - smoothstep(.0, .15, cld) * .5;
    u = (uv - vec2(0.0, 0.25)) * vec2(1, 15);
    id = floor(u);

    for (float i = 1.; i > -1.; i--) {
        if (id.y + i < 0.0) {
            lc = fract(u) - .5;
            lc.y = (lc.y + (sin(uv.x * 8. - time * 0.8 + id.y + i)) * .3 - i) * 4.;
            h = hash12(vec2(id.y + i, floor(lc.y)));
            xd = 6. + h * 4.;
            yd = 30.;
            lc.x = uv.x * xd + sh.x * 9.;
            lc.x += sin(time * (.2 + h * 1.5)) * .5;
            h = .8 * smoothstep(5., .0, abs(floor(lc.x))) * cld + .1;

            f = mix(f, mix(MountainLayerFourCol, MountainLayerThreecol, h), st(lc.y, 0., sm * yd));
            lc += noise(lc * vec2(3, .5)) * vec2(.1, .6);

            vec3 strokeCol = hue(hash12(floor(lc)) * .1 + .35).rgb * (1.2 + floor(lc.y) * .17);
            f = mix(f, mix(strokeCol, FieldMid, h), st(lc.y, 0., sm * xd) * st(abs(fract(lc.x) - .5), .48, sm * xd) * st(abs(fract(lc.y) - .5), .3, sm * yd));
        }
    }
    return f;
}

vec3 getPaintedWaterAndSand(vec2 g, vec2 r, float time, vec3 bgCol) {
    vec2 uv = (g + g - r) / r.y;
    vec2 sun_pos = vec2(r.x / r.y * 0.35, -0.15);
    vec2 sh = rotate2D(sun_pos, noise(uv + time * .25) * .3);
    float sm = 3. / r.y;
    vec3 f = bgCol;

    float waterZone = smoothstep(0.16, 0.14, uv.y);
    f = mix(f, vec3(0.05, 0.15, 0.3), waterZone);

    float cld = noise(-sh * vec2(.5, 1) - vec2(time * .2, 0));
    cld = 1. - smoothstep(.0, .15, cld) * .5;

    vec2 u = (uv - vec2(0.0, 0.15)) * vec2(1, 15);
    vec2 id = floor(u);

    for (float i = 1.; i > -1.; i--) {
        if (id.y + i < 0.0) {
            vec2 lc = fract(u) - .5;
            lc.y = (lc.y + (sin(uv.x * 12. - time * 3. + id.y + i)) * .25 - i) * 4.;
            float h = hash12(vec2(id.y + i, floor(lc.y)));

            float xd = 6. + h * 4.;
            float yd = 30.;
            lc.x = uv.x * xd + sh.x * 9.;
            lc.x += sin(time * (.5 + h * 2.)) * .5;
            h = .8 * smoothstep(5., .0, abs(floor(lc.x))) * cld + .1;

            vec3 waterBase = mix(vec3(0.0, 0.15, 0.4), vec3(0.6, 0.4, 0.1), h);
            f = mix(f, waterBase, st(lc.y, 0., sm * yd));
            lc += noise(lc * vec2(3, .5)) * vec2(.1, .6);

            vec3 strokeCol = mix(hue(hash12(floor(lc)) * .1 + .56).rgb * (1.2 + floor(lc.y) * .17), vec3(1.0, 0.8, 0.2), h);
            f = mix(f, strokeCol, st(lc.y, 0., sm * xd) * st(abs(fract(lc.x) - .5), .48, sm * xd) * st(abs(fract(lc.y) - .5), .3, sm * yd));
        }
    }

    vec2 u_sand = uv + noise(uv * 2.) * .1 + vec2(0, sin(uv.x * 1.2 + 3.) * .2 + 0.7);
    vec3 sandCol = vec3(0.85, 0.75, 0.50);
    vec3 sandDark = vec3(0.60, 0.45, 0.25);
    f = mix(f, sandDark * 0.7, step(u_sand.y, .0));

    float xd_s = 60.;
    u_sand = u_sand * vec2(xd_s, xd_s / 3.5);

    if (u_sand.y < 1.2) {
        for (float y = 0.; y > -3.; y--) {
            for (float x = -2.; x < 3.; x++) {
                vec2 id_s = floor(u_sand) + vec2(x, y);
                vec2 lc_s = (fract(u_sand) + vec2(1. - x, -y)) / vec2(5, 3);
                float h_s = (hash12(id_s) - .5) * .25 + .5;

                lc_s -= vec2(.3, .5 - h_s * .4);
                lc_s.x += sin(((time * 0.5 + h_s * 2. - id_s.x * .05 - id_s.y * .05) * 1.1 + id_s.y * .5) * 2.) * (lc_s.y + .5) * .2;
                vec2 t_s = abs(lc_s) - vec2(.03, .5 - h_s * .5);
                float l = length(max(t_s, 0.)) + min(max(t_s.x, t_s.y), 0.);

                l -= noise(lc_s * 7. + id_s) * .1;

                vec4 C = vec4(sandDark * 0.5, st(l, .1, sm * xd_s * .09));
                vec4 fg = vec4(sandCol * (1.1 + lc_s.y * 1.2) * (1.2 - h_s * 1.2), 1.);
                C = mix(C, fg, st(l, .04, sm * xd_s * .09));

                f = mix(f, C.rgb, C.a * step(id_s.y, -1.));
            }
        }
    }
    return f;
}

float sdTri(vec2 p, float base, float tip, float width) {
    float dY = max(base - p.y, p.y - tip);
    float frac = clamp((p.y - base) / (tip - base), 0.0, 1.0);
    float curWidth = width * (1.0 - frac);
    float dX = abs(p.x) - curWidth;
    return max(dX, dY);
}

float sdTree(vec2 p, vec2 pos, float scale) {
    vec2 q = (p - pos) / scale;
    q.x -= sin(u_Time * 1.5 + pos.x * 10.0) * 0.02 * max(0.0, q.y);
    float d = max(abs(q.x) - 0.03, max(-q.y, q.y - 0.2));
    d = min(d, sdTri(q, 0.1, 0.45, 0.25));
    d = min(d, sdTri(q, 0.25, 0.65, 0.2));
    d = min(d, sdTri(q, 0.45, 0.85, 0.15));
    d = min(d, sdTri(q, 0.65, 1.0, 0.1));
    return d * scale;
}

float getLand(vec2 uv, float aspect) {
    float d = 1.0;

    // Left Landmass
    float w1 = 0.05 * exp(-12.0 * pow(uv.x - aspect * 0.1, 2.0));
    float c1 = 0.36 - 0.01 * uv.x;
    // Taper Cutoff: smoothstep adds distance penalty when width drops below 0.008
    float land1 = abs(uv.y - c1) - w1 + smoothstep(0.008, 0.0, w1);
    d = min(d, land1);

    // Center Island
    float w2 = 0.03 * exp(-30.0 * pow(uv.x - aspect * 0.52, 2.0));
    float c2 = 0.48;
    // Taper Cutoff
    float land2 = abs(uv.y - c2) - w2 + smoothstep(0.008, 0.0, w2);
    d = min(d, land2);

    return d;
}

float getTrees(vec2 uv, float aspect) {
    float d = 1.0;
    d = min(d, sdTree(uv, vec2(aspect * 0.05, 0.41), 0.18));
    d = min(d, sdTree(uv, vec2(aspect * 0.11, 0.40), 0.22));
    d = min(d, sdTree(uv, vec2(aspect * 0.17, 0.38), 0.14));
    d = min(d, sdTree(uv, vec2(aspect * 0.23, 0.36), 0.09));
    d = min(d, sdTree(uv, vec2(aspect * 0.48, 0.508), 0.07));
    d = min(d, sdTree(uv, vec2(aspect * 0.51, 0.51), 0.10));
    d = min(d, sdTree(uv, vec2(aspect * 0.54, 0.50), 0.06));
    return d;
}

vec4 getPaintedTrees(vec2 uv, vec2 r, float time, float aspect) {
    float sm = 3.0 / r.y;
    float treeDist = getTrees(uv, aspect);

    float treeMask = smoothstep(0.02, -0.01, treeDist);
    float coreMask = smoothstep(0.003, 0.0, treeDist);

    if (treeMask <= 0.0) return vec4(0.0);

    vec3 baseCol = vec3(0.05, 0.1, 0.08);
    vec4 C = vec4(baseCol, coreMask);

    float xd = 160.0;
    float yd = 45.0;

    for (float i = 0.0; i < 3.0; i++) {
        vec2 u = uv;
        u.x -= sin(time * 1.5 + uv.x * 10.0) * 0.02 * max(0.0, uv.y - 0.4);
        u += noise(u * 12.0 + vec2(time * 0.3, i * 5.0)) * 0.025;

        vec2 t = u * vec2(xd, yd) + vec2(i * 11.3, i * 17.7);

        vec2 id = floor(t);
        vec2 lc = fract(t);
        float h = hash12(id);

        if (h < 0.4) continue;

        float taper = mix(1.2, 0.2, lc.y);

        float outline = st(abs(lc.x - 0.5), 0.35 * taper, sm * xd) * st(abs(lc.y - 0.5), 0.48, sm * yd);
        float body = st(abs(lc.x - 0.5), 0.22 * taper, sm * xd) * st(abs(lc.y - 0.5), 0.4, sm * yd);

        vec3 strokeCol = mix(vec3(0.05, 0.25, 0.1), vec3(0.15, 0.4, 0.15), h);
        if (h > 0.8) strokeCol = mix(strokeCol, vec3(0.3, 0.5, 0.15), 0.6);
        if (h < 0.55) strokeCol = mix(strokeCol, vec3(0.0, 0.15, 0.2), 0.7);

        float strokeAlpha = outline * treeMask;

        C.rgb = mix(C.rgb, vec3(0.02, 0.04, 0.03), strokeAlpha);
        C.rgb = mix(C.rgb, strokeCol, body * treeMask);
        C.a = max(C.a, strokeAlpha);
    }
    return C;
}

vec4 getIslandGrass(vec2 uv, vec2 r, float time, float aspect) {
    float sm = 3.0 / r.y;
    vec4 O = vec4(0.0);

    vec2 u = uv + noise(uv * 2.0) * 0.05;
    float xd = 160.0;
    u = u * vec2(xd, xd / 2.0);

    vec3 f_col = mix(vec3(0.3, 0.65, 0.2), vec3(0.5, 0.8, 0.2), sin(time * 0.5) * 0.5 + 0.5);
    vec3 cRock = vec3(0.08, 0.1, 0.12);

    for (float y = 0.0; y > -4.0; y--) {
        for (float x = -2.0; x < 3.0; x++) {
            vec2 id = floor(u) + vec2(x, y);
            vec2 root_uv = id / vec2(xd, xd / 2.0);

            // Re-apply the taper cutoff logic here so grass respects the boundary
            float w1 = 0.05 * exp(-12.0 * pow(root_uv.x - aspect * 0.1, 2.0));
            float c1 = 0.36 - 0.01 * root_uv.x;
            float land1 = abs(root_uv.y - c1) - w1 + smoothstep(0.008, 0.0, w1);

            float w2 = 0.03 * exp(-30.0 * pow(root_uv.x - aspect * 0.52, 2.0));
            float c2 = 0.48;
            float land2 = abs(root_uv.y - c2) - w2 + smoothstep(0.008, 0.0, w2);

            float landDist = min(land1, land2);
            float rootAlpha = smoothstep(0.01, -0.01, landDist);

            if (rootAlpha <= 0.0) continue;

            vec2 lc = (fract(u) + vec2(1.0 - x, -y)) / vec2(5.0, 3.0);
            float h = (hash12(id) - 0.5) * 0.25 + 0.5;

            lc -= vec2(0.3, 0.5 - h * 0.4);
            lc.x += sin(((time * 1.5 + h * 2.0 - id.x * 0.05 - id.y * 0.05) * 1.1 + id.y * 0.5) * 2.0) * (lc.y + 0.5) * 0.25;

            vec2 t = abs(lc) - vec2(0.02, 0.5 - h * 0.5);
            float l = length(max(t, 0.0)) + min(max(t.x, t.y), 0.0);

            l -= noise(lc * 7.0 + id) * 0.1;

            vec4 C = vec4(cRock, st(l, 0.1, sm * xd * 0.09));
            vec4 fg = vec4(f_col * (1.2 + lc.y * 2.0) * (1.8 - h * 2.5), 1.0);
            C = mix(C, fg, st(l, 0.04, sm * xd * 0.09));

            C.a *= rootAlpha;
            O = mix(O, C, C.a);
        }
    }
    return O;
}

void mainImage() {
    vec2 uv = gl_FragCoord.xy / u_Resolution.xy;
    float aspect = u_Resolution.x / u_Resolution.y;
    uv.x *= aspect;

    // 1. Background (Painted Sky & Mountains)
    vec3 col = getPaintedSkyAndMountains(gl_FragCoord.xy, u_Resolution.xy, u_Time);

    // 2. Midground/Foreground (Painted Water & Sand)
    col = getPaintedWaterAndSand(gl_FragCoord.xy, u_Resolution.xy, u_Time, col);

    // 3. Composite Land Base
    float landDist = getLand(uv, aspect);
    float landMask = smoothstep(0.003, 0.0, landDist);
    vec3 cRock = vec3(0.08, 0.1, 0.12);
    col = mix(col, cRock, landMask);

    // 4. Composite Painted Trees First
    vec4 trees = getPaintedTrees(uv, u_Resolution.xy, u_Time, aspect);
    col = mix(col, trees.rgb, trees.a);

    // 5. Composite Full-Coverage Island Grass (Overlaps the trees to plant them)
    vec4 grass = getIslandGrass(uv, u_Resolution.xy, u_Time, aspect);
    col = mix(col, grass.rgb, grass.a);

    o_Color = vec4(col, 1.0);
}
