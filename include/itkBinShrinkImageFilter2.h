/*=========================================================================
 *
 *  Copyright Bradley Lowekamp
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkBinShrinkImageFilter2_h
#define itkBinShrinkImageFilter2_h

#include "itkShrinkImageFilter.h"

#if ITK_VERSION_MAJOR < 4
#define ThreadIdType int
#endif

namespace itk
{

/** \class BinShrinkImageFilter2
 * \brief Reduce the size of an image by an integer factor in each
 * dimension.
 *
 * BinShrinkImageFilter2 reduces the size of an image by an integer factor
 * in each dimension. The algorithm implemented is a mean or box
 * filter subsample.
 *
 * The output image size in each dimension is given by:
 *
 * outputSize[j] = max( vcl_floor(inputSize[j]/shrinkFactor[j]), 1 );
 *
 * NOTE: The physical centers of the input and output will be the
 * same. Because of this, the Origin of the output may not be the same
 * as the Origin of the input.
 * Since this filter produces an image which is a different
 * resolution, origin and with different pixel spacing than its input
 * image, it needs to override several of the methods defined
 * in ProcessObject in order to properly manage the pipeline execution model.
 * In particular, this filter overrides
 * ProcessObject::GenerateInputRequestedRegion() and
 * ProcessObject::GenerateOutputInformation().
 *
 * This filter is implemented as a multithreaded filter.  It provides a
 * ThreadedGenerateData() method for its implementation.
 *
 * \ingroup ITKBinShrink
 * \ingroup Streamed
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT BinShrinkImageFilter2 :
  public         ShrinkImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinShrinkImageFilter2                       Self;
  typedef ShrinkImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>                          Pointer;
  typedef SmartPointer<const Self>                    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinShrinkImageFilter2, ShrinkImageFilter);

  /** Typedef to images */
  typedef TOutputImage                          OutputImageType;
  typedef TInputImage                           InputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;

  typedef typename TOutputImage::IndexType  OutputIndexType;
  typedef typename TInputImage::IndexType   InputIndexType;
  typedef typename TOutputImage::OffsetType OutputOffsetType;

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** ImageDimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension );
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension );

  /** BinShrinkImageFilter2 needs a larger input requested region than the output
   * requested region.  As such, BinShrinkImageFilter2 needs to provide an
   * implementation for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   * \sa ProcessObject::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() ITK_OVERRIDE;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
                  (Concept::Convertible<typename TInputImage::PixelType, typename TOutputImage::PixelType>) );
  itkConceptMacro(SameDimensionCheck,
                  (Concept::SameDimension<ImageDimension, OutputImageDimension>) );
  /** End concept checking */
#endif

protected:
  BinShrinkImageFilter2();
  ~BinShrinkImageFilter2() {
  }

  /** BinShrinkImageFilter2 can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            ThreadIdType threadId ) ITK_OVERRIDE;

private:
  BinShrinkImageFilter2(const Self&); //purposely not implemented
  void operator=(const Self&);        //purposely not implemented

  OutputOffsetType ComputeOffsetIndex(void);

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinShrinkImageFilter2.hxx"
#endif

#endif
