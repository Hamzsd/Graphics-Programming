#version 120

varying vec4 p;

void main()
{
	int l = 0;
	float s = 0;

	while(s == 0 && l < 5)
	{
		l++;
		for (int i = 0; i < 9 - l; i++)
		{
			float x = sin(cos(i*2*l - p.z * 2));
			float y = sin(cos(i * 9 * l +  p.z * 3));
			s += (sin(p.z * 10/l + i + l) * 0.2 * l + 0.04)/sqrt((p.x-x) + (p.y - y) * (p.y - y));
		}
		if ( s < 0.3/ l || s > 0.8) 
			s = 0;
	}
	float si = s*15;
	s = si/15.0;
	vec3 c = vec3(sin(p.z*8 + p.x) + 0.3, cos(p.z*8+p.y)+0.3, 0.7)*s/l;
	gl_FragColor = vec4(c, 1);
}
