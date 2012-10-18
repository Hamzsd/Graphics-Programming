   #version 120

   varying vec4 p;

   void main()
   {
	p = gl_Position = gl_Vertex;
	p.z = gl_Color.x;
   }
   
  
