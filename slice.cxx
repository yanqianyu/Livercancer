#include "itkImage.h"
#include "itkMath.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageSliceConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include <vector>

int main(){
    typedef unsigned short PixelType;
    typedef unsigned char PixelWriterType;
    typedef itk:: Image<PixelWriterType,2> ImageType2D;
    typedef itk:: Image<PixelType,3> ImageType3D;
    
    typedef itk:: ImageLinearIteratorWithIndex<ImageType2D> LinearIteratorType;
    typedef itk:: ImageSliceConstIteratorWithIndex<ImageType3D> SliceIteratorType;
    
    typedef itk::ImageFileReader< ImageType3D > ReaderType;
    
    typedef itk::ImageSeriesWriter< ImageType3D, ImageType2D > WriterType;
    
    typedef itk::ImageFileWriter<ImageType2D> WriterType2;
    ReaderType::Pointer reader=ReaderType::New();
    WriterType::Pointer writer = WriterType::New();
    
    writer->SetInput( reader->GetOutput() );

    ImageType3D::ConstPointer inputImage;
    //ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( "/Users/apple/Desktop/DIF_NOD_001.nii" );
    try
    {
        reader->Update();
        inputImage = reader->GetOutput();
    }
    catch ( itk::ExceptionObject &err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    
    typedef itk::NumericSeriesFileNames    NameGeneratorType;
    
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    
    std::string directory[3];
    directory[0]="/Users/apple/Desktop/slices/x/";
    directory[1]="/Users/apple/Desktop/slices/y/";
    directory[2]="/Users/apple/Desktop/slices/z/";
    
    for(int i=0;i<3;i++){
        std::string format = directory[i];
        format += "%03d.";
        format += "jpg";   // filename extension
        
        nameGenerator->SetSeriesFormat( format.c_str() );
        ImageType3D::ConstPointer inputImage = reader->GetOutput();
        ImageType3D::RegionType   _region     = inputImage->GetLargestPossibleRegion();
        ImageType3D::IndexType    _start      = _region.GetIndex();
        ImageType3D::SizeType     _size       = _region.GetSize();
        
        writer->SetFileNames( nameGenerator->GetFileNames() );

        const unsigned int firstSlice = _start[i];
        const unsigned int lastSlice  = _start[i] + _size[i] - 1;
        
        nameGenerator->SetStartIndex( firstSlice );
        nameGenerator->SetEndIndex( lastSlice );
        nameGenerator->SetIncrementIndex( 1 );

        std::vector< std::string >  nameVector = nameGenerator->GetFileNames();
        
    unsigned int direction[2];
        direction[0]=0;
        direction[1]=1;
    for (int j = 0, k = 0; j< 3; ++j )
    {
        if (j != i)
        {
            direction[k] = j;
            k++;
        }
    }

    ImageType2D::RegionType region;
    ImageType2D::RegionType::SizeType size;
    ImageType2D::RegionType::IndexType index;
    
    ImageType3D::RegionType requestedRegion = inputImage->GetRequestedRegion();
    
    index[ direction[0] ]    = requestedRegion.GetIndex()[ direction[0] ];
    index[ 1- direction[0] ] = requestedRegion.GetIndex()[ direction[1] ];
    size[ direction[0] ]     = requestedRegion.GetSize()[  direction[0] ];
    size[ 1- direction[0] ]  = requestedRegion.GetSize()[  direction[1] ];
    
    region.SetSize( size );
    region.SetIndex( index );
    
    ImageType2D::Pointer outputImage = ImageType2D::New();
    
    outputImage->SetRegions( region );
    outputImage->Allocate();
    
    SliceIteratorType  inputIt(  inputImage, inputImage->GetRequestedRegion() );
    LinearIteratorType outputIt( outputImage,
                                outputImage->GetRequestedRegion() );
    
    inputIt.SetFirstDirection(  direction[1] );
    inputIt.SetSecondDirection( direction[0] );
    
    outputIt.SetDirection( 1 - direction[0] );
        
    inputIt.GoToBegin();
    outputIt.GoToBegin();
        
    std::vector< std::string >::iterator it = nameVector.begin();
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    while( !inputIt.IsAtEnd() )
    {
        while ( !inputIt.IsAtEndOfSlice() && it != nameVector.end())
        {
            while ( !inputIt.IsAtEndOfLine() )
            {
                PixelWriterType outputPixel;
                outputPixel = (inputIt.Get()+840) * 255/1290;
                outputIt.Set( outputPixel );

                ++inputIt;
                ++outputIt;
            }
            outputIt.NextLine();
            inputIt.NextLine();
        }

    
    WriterType2::Pointer writer = WriterType2::New();
    writer->SetFileName( *it);
    ++it;
        
    writer->SetInput(outputImage);
    try
    {
        writer->Update();
    }
    catch ( itk::ExceptionObject &err)
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
        outputIt.GoToBegin();
        inputIt.NextSlice();
        
    }
    }
    return EXIT_SUCCESS;
}
