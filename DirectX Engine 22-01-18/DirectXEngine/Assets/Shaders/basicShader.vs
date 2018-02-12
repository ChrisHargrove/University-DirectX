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
	float2 textureCoord 	: TEXCOORD0;
};


/*******************************************************************************************************************
	Data to be sent to the pixel shader
*******************************************************************************************************************/
struct PixelOutput
{
    float4 position 		: SV_POSITION;
	float2 textureCoord 	: TEXCOORD0;
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
    pixelOutput.textureCoord = vertexInput.textureCoord;
	
	//-------------------------------------------- Send the data to the pixel shader
    return pixelOutput;
}