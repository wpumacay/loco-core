
#include <loco.h>
#include <gtest/gtest.h>

std::vector<float> create_heights_data( ssize_t nWidthSamples, ssize_t nDepthSamples )
{
    const float dx = 2.0f / ( nWidthSamples - 1 );
    const float dy = 2.0f / ( nDepthSamples - 1 );

    std::vector<float> zz;
    for ( ssize_t i = 0; i < nDepthSamples; i++ )
    {
        for ( ssize_t j = 0; j < nWidthSamples; j++ )
        {
            float x = -1.0f + j * dx; // [-1, 1]
            float y = -1.0f + i * dy; // [-1, 1]
            float z = 1.0f + std::sqrt( x * x + y * y );
            zz.push_back( z );
        }
    }
    return zz;
}

// Run till completion (check that THeightFieldData struct is ok)
TEST( TestLocoDataBase, TestHfieldDataBase )
{
    const ssize_t nWidthSamples = 10;
    const ssize_t nDepthSamples = 20;
    auto hfdata = loco::THeightFieldData();
    hfdata.nWidthSamples = nWidthSamples;
    hfdata.nDepthSamples = nDepthSamples;
    hfdata.heights = create_heights_data( nWidthSamples, nDepthSamples );
}

// Run till completion (check that base TShapeData struct is ok)
TEST( TestLocoDataBase, TestShapeDataBase )
{
    const ssize_t nWidthSamples = 10;
    const ssize_t nDepthSamples = 20;
    auto shapeData = loco::TShapeData();
    shapeData.type = loco::eShapeType::HFIELD;
    shapeData.size = { 1.0f, 1.0f, 2.0f };
    shapeData.localTransform.setIdentity();
    shapeData.mesh_data.filename = "";
    shapeData.hfield_data.nWidthSamples = nWidthSamples;
    shapeData.hfield_data.nDepthSamples = nDepthSamples;
    shapeData.hfield_data.heights = create_heights_data( nWidthSamples, nDepthSamples );
}