3、shader 变量的qualifier:
	默认; //无修饰符，普通变量读写， 与外界无连接；
	const; //常量 const vec3 zAxis = vec3(0.0, 0.0, 1.0);
	attribute; //申明传给vertex shader的变量；只读；不能为array或struct；attribute vec4 position;
	uniform; //表明整个图元处理中值相同；只读； uniform vec4 lightPos;
	varying; //被差值；读写； varying vec3 normal;
	in, out, inout;
14、shader变量的精度; //
	highp, mediump, lowp
 
15、shader内置变量; //
	gl_Position; //用于vertex shader, 写顶点位置；被图元收集、裁剪等固定操作功能所使用；
				 其内部声明是; //highp vec4 gl_Position;
	gl_PointSize; //用于vertex shader, 写光栅化后的点大小，像素个数；
				 其内部声明是; //mediump float gl_Position;
	gl_FragColor; //用于Fragment shader，写fragment color；被后续的固定管线使用；
				  mediump vec4 gl_FragColor;
	gl_FragData; //用于Fragment shader，是个数组，写gl_FragData[n] 为data n；被后续的固定管线使用；
				  mediump vec4 gl_FragData[gl_MaxDrawBuffers];
	gl_FragColor和gl_FragData是互斥的，不会同时写入；
	gl_FragCoord; //用于Fragment shader,只读， Fragment相对于窗口的坐标位置 x,y,z,1/w; 这个是固定管线图元差值后产生的；z 是深度值; mediump vec4 gl_FragCoord;
	gl_FrontFacing; //用于判断 fragment是否属于 front-facing primitive；只读；
					bool gl_FrontFacing;
	gl_PointCoord; //仅用于 point primitive; mediump vec2 gl_PointCoord;
16、shader内置常量; //
	const mediump int gl_MaxVertexAttribs = 8;
	const mediump int gl_MaxVertexUniformVectors = 128;
	const mediump int gl_MaxVaryingVectors = 8;
	const mediump int gl_MaxVertexTextureImageUnits = 0;
	const mediump int gl_MaxCombinedTextureImageUnits = 8;
	const mediump int gl_MaxTextureImageUnits = 8;
	const mediump int gl_MaxFragmentUnitformVectors = 16;
	const mediump int gl_MaxDrawBuffers = 1;
17、shader内置函数; //
	一般默认都用弧度；
	radians(degree) ; //角度变弧度；
	degrees(radian) ; //弧度变角度；
	sin(angle), cos(angle), tan(angle)
	asin(x); //arc sine, 返回弧度 [-PI/2, PI/2];
	acos(x); //arc cosine,返回弧度 [0, PI];
	atan(y, x); //arc tangent, 返回弧度 [-PI, PI];
	atan(y/x); //arc tangent, 返回弧度 [-PI/2, PI/2];
   
	pow(x, y); //x的y次方；
	exp(x); //指数, log(x); //
	exp2(x); //2的x次方， log2(x):
	sqrt(x); //x的根号； inversesqrt(x); //x根号的倒数
   
	abs(x); //绝对值
	sign(x); //符号, 1, 0 或 -1
	floor(x); //底部取整
	ceil(x); //顶部取整
	fract(x); //取小数部分
	mod(x, y); //取模， x - y*floor(x/y)
	min(x, y); //取最小值
	max(x, y); //取最大值
	clamp(x, min, max); // min(max(x, min), max);
	mix(x, y, a); //x, y的线性混叠， x(1-a) + y*a;
	step(edge, x); //如 x
	smoothstep(edge0, edge1, x); //threshod  smooth transition时使用。 edge0<=edge0时为0.0， x>=edge1时为1.0
   
	length(x); //向量长度
	distance(p0, p1); //两点距离， length(p0-p1);
	dot(x, y); //点积，各分量分别相乘 后 相加
	cross(x, y); //差积，x[1]*y[2]-y[1]*x[2], x[2]*y[0] - y[2]*x[0], x[0]*y[1] - y[0]*x[1]
	normalize(x); //归一化， length(x)=1;
	faceforward(N, I, Nref); //如 dot(Nref, I)< 0则N, 否则 -N
	reflect(I, N); //I的反射方向， I -2*dot(N, I)*N, N必须先归一化
	refract(I, N, eta); //折射，k=1.0-eta*eta*(1.0 - dot(N, I) * dot(N, I)); 如k<0.0 则0.0，否则 eta*I - (eta*dot(N, I)+sqrt(k))*N
   
	matrixCompMult(matX, matY); //矩阵相乘, 每个分量 自行相乘， 即 r[i][j] = x[i][j]*y[i][j];
							   矩阵线性相乘，直接用 *
	
	lessThan(vecX, vecY); //向量 每个分量比较 x < y
	lessThanEqual(vecX, vecY); //向量 每个分量比较 x<=y
	greaterThan(vecX, vecY); //向量 每个分量比较 x>y
	greaterThanEqual(vecX, vecY); //向量 每个分量比较 x>=y
	equal(vecX, vecY); //向量 每个分量比较 x==y
	notEqual(vecX, vexY); //向量 每个分量比较 x!=y
	any(bvecX); //只要有一个分量是true， 则true
	all(bvecX); //所有分量是true， 则true
	not(bvecX); //所有分量取反
   
	texture2D(sampler2D, coord); //texture lookup
	texture2D(sampler2D, coord, bias); //LOD bias, mip-mapped texture
	texture2DProj(sampler2D, coord):
	texture2DProj(sampler2D, coord, bias):
	texture2DLod(sampler2D, coord, lod):
	texture2DProjLod(sampler2D, coord, lod):
	textureCube(samplerCube, coord):
	textureCube(samplerCube, coord, bias):
	textureCubeLod(samplerCube, coord, lod):
