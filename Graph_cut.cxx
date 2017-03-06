#include "itkConnectedThresholdImageFilter.h"
#include "itkImage.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( )
{
    
    typedef   float           InternalPixelType;
    const     unsigned int    Dimension = 2;
    typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
    
    typedef unsigned char                            OutputPixelType;
    typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
    typedef itk::CastImageFilter< InternalImageType, OutputImageType >
    CastingFilterType;
    CastingFilterType::Pointer caster = CastingFilterType::New();
    
    typedef  itk::ImageFileReader< InternalImageType > ReaderType;
    typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;
    
    ReaderType::Pointer reader = ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    reader->SetFileName( "/Users/apple/Desktop/example.jpg" );
    writer->SetFileName( "/Users/apple/Desktop/first.jpg" );

    typedef itk::CurvatureFlowImageFilter< InternalImageType, InternalImageType >
    CurvatureFlowImageFilterType;
    
    CurvatureFlowImageFilterType::Pointer smoothing =
    CurvatureFlowImageFilterType::New();

    typedef itk::ConnectedThresholdImageFilter< InternalImageType,
    InternalImageType > ConnectedFilterType;
    
    ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
    
    smoothing->SetInput( reader->GetOutput() );
    connectedThreshold->SetInput( smoothing->GetOutput() );
    caster->SetInput( connectedThreshold->GetOutput() );
    writer->SetInput( caster->GetOutput() );

    smoothing->SetNumberOfIterations( 5 );
    smoothing->SetTimeStep( 0.125 );

    const InternalPixelType lowerThreshold = 20;
    const InternalPixelType upperThreshold = 70;
    
    connectedThreshold->SetLower(  lowerThreshold  );
    connectedThreshold->SetUpper(  upperThreshold  );

    connectedThreshold->SetReplaceValue( 255 );
    
    InternalImageType::IndexType  index;
    
    index[0] = 250;
    index[1] = 250;
    
    
    connectedThreshold->SetSeed( index );
    
    try
    {
        writer->Update();
    }
    catch( itk::ExceptionObject & excep )
    {
        std::cerr << "Exception caught !" << std::endl;
        std::cerr << excep << std::endl;
    }

    return EXIT_SUCCESS;
}

