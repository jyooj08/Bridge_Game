
uniform float Kbias;


void main()
{
	// do nothing
	//if(gl_FragDepth >= gl_FragCoord.z || gl_FragDepth ==0)
	gl_FragDepth = gl_FragCoord.z+Kbias;
	//FragColor.r = gl_FragCoord.z;
	//frag_out = normalize(frag_pos)/frag_pos.w;
	//frag_out.z = frag_pos.z/frag_pos.w*0.5 + 0.5;
}