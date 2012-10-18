   #version 330

   varying vec3 p;

   void main()
   {
	p = gl_Position = gl_vertex;
	p.z = gl_Color.x;
   }
   
  
