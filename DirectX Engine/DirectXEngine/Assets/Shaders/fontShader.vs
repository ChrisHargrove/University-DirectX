
/*******************************************************************************************************************
	Vertex data coming in from the models
*******************************************************************************************************************/
struct VertexInput
{
    float4 position : POSITION;
    float2 tex		: TEXCOORD0;
};


/*******************************************************************************************************************
	Data to be sent to the pixel shader
*******************************************************************************************************************/
struct PixelOutput
{
    float4 position : SV_POSITION;
    float2 tex		: TEXCOORD0;
};


/*******************************************************************************************************************
	Main Function
*******************************************************************************************************************/
PixelOutput VertexMain(VertexInput vertexInput)
{
    PixelOutput pixelOutput;

	//-------------------------------------------- Change the position vector to be 4 units for proper matrix calculations
    pixelOutput.position = vertexInput.position;
    
	//-------------------------------------------- Store the texture coordinates for the pixel shader to use
	pixelOutput.tex = vertexInput.tex;
    
	//-------------------------------------------- Send the data to the pixel shader
    return pixelOutput;
}