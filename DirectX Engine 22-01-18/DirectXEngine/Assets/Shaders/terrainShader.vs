/*******************************************************************************************************************
	Constant buffer data coming in from the CPU
*******************************************************************************************************************/
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


/*******************************************************************************************************************
	Vertex data coming in from the models
*******************************************************************************************************************/
struct VertexInput
{
    float4 position 		: POSITION;
	float3 normal 			: NORMAL;
};


/*******************************************************************************************************************
	Data to be sent to the pixel shader
*******************************************************************************************************************/
struct PixelOutput
{
    float4 position 		: SV_POSITION;
	float3 normal 			: NORMAL;
};


/*******************************************************************************************************************
	Main Function
*******************************************************************************************************************/
PixelOutput VertexMain(VertexInput vertexInput)
{
    PixelOutput pixelOutput;
    
	//-------------------------------------------- Change the position vector to be 4 units for proper matrix calculations
    vertexInput.position.w = 1.0f;

	//--------------------------------------------  Calculate the position of the vertex against the world, view, and projection matrices
    pixelOutput.position = mul(vertexInput.position, worldMatrix);
    pixelOutput.position = mul(pixelOutput.position, viewMatrix);
    pixelOutput.position = mul(pixelOutput.position, projectionMatrix);
    
	//--------------------------------------------  Store the input texture for the pixel shader to use
    //pixelOutput.textureCoord = vertexInput.textureCoord;
	   
	// Calculate the normal vector against the world matrix only.
    pixelOutput.normal = mul(vertexInput.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    pixelOutput.normal = normalize(pixelOutput.normal);
	
	//-------------------------------------------- Send the data to the pixel shader
    return pixelOutput;
}