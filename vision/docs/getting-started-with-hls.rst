.. _ariaid-title1:

Getting Started with HLS
========================

The xfOpenCV library can be used to build applications in Vivado® HLS.
This section provides details on how the xfOpenCV library components can
be integrated into a design in Vivado HLS 2019.1. This section of the
document provides steps on how to run a single library component through
the Vivado HLS 2019.1 use flow which includes, C-simulation,
C-synthesis, C/RTL co-simulation, and exporting the RTL as an IP.

You are required to do the following changes to facilitate proper
functioning of the use model in Vivado HLS 2019.1:

#. Use of appropriate compile-time options - When using the xfOpenCV
   functions in HLS, the ``-D__SDSVHLS__`` and ``-std=c++0x`` options
   need to be provided at the time of compilation:
#. Specifying interface pragmas to the interface level arguments - For
   the functions with top level interface arguments as pointers (with
   more than one read/write access), the ``m_axi`` Interface pragma must
   be specified. For example,

   .. code:: pre

      void lut_accel(xf::Mat<TYPE, HEIGHT, WIDTH, NPC1> &imgInput, xf::Mat<TYPE, HEIGHT, WIDTH, NPC1> &imgOutput, unsigned char *lut_ptr)
      {
      #pragma HLS INTERFACE m_axi depth=256 port=lut_ptr offset=direct bundle=lut_ptr
          xf::LUT< TYPE, HEIGHT, WIDTH, NPC1> (imgInput,imgOutput,lut_ptr);
      }

.. _ariaid-title2:

HLS Standalone Mode
-------------------

The HLS standalone mode can be operated using the following two modes:

#. Tcl Script Mode
#. GUI Mode

.. _ariaid-title3:

Tcl Script Mode
~~~~~~~~~~~~~~~

Use the following steps to operate the HLS Standalone Mode using Tcl
Script:

#. In the Vivado® HLS tcl script file, update the cflags in all the
   add_files sections.
#. Append the path to the xfOpenCV/include directory, as it contains all
   the header files required by the library.
#. Add the ``-D__SDSVHLS__`` and ``-std=c++0x`` compiler flags.

Note: When using Vivado HLS in the Windows operating system, provide the
``-std=c++0x`` flag only for C-Sim and Co-Sim. Do not include the flag
when performing synthesis.

For example:

Setting flags for source files:

.. code:: pre

   add_files xf_dilation_accel.cpp -cflags "-I<path-to-include-directory> -D__SDSVHLS__ -std=c++0x" 

Setting flags for testbench files:

.. code:: pre

   add_files -tb xf_dilation_tb.cpp -cflags "-I<path-to-include-directory> -D__SDSVHLS__ -std=c++0x"

.. _ariaid-title4:

GUI Mode
~~~~~~~~

Use the following steps to operate the HLS Standalone Mode using GUI:

#. Open Vivado® HLS in GUI mode and create a new project
#. Specify the name of the project. For example - Dilation.
#. Click Browse to enter a workspace folder used to store your projects.
#. Click Next.
#. Under the source files section, add the accel.cpp file which can be
   found in the examples folder. Also, fill the top function name (here
   it is dilation_accel).
#. Click Next.
#. Under the test bench section add tb.cpp.
#. Click Next.
#. Select the clock period to the required value (10ns in example).
#. Select the suitable part. For example, ``xczu9eg-ffvb1156-2-i``.
#. Click Finish.
#. Right click on the created project and select Project Settings.
#. In the opened tab, select Simulation.
#. Files added under the Test Bench section will be displayed. Select a
   file and click Edit CFLAGS.
#. Enter
   ``-I<path-to-include-directory>                         -D__SDSVHLS__ -std=c++0x``.
   Note: When using Vivado HLS in the Windows operating system, make
   sure to provide the ``-std=c++0x`` flag only for C-Sim and Co-Sim. Do
   not include the flag when performing synthesis.
#. Select Synthesis and repeat the above step for all the displayed
   files.
#. Click OK.
#. Run the C Simulation, select Clean Build and specify the required
   input arguments.
#. Click OK.
#. All the generated output files/images will be present in the
   solution1->csim->build.
#. Run C synthesis.
#. Run co-simulation by specifying the proper input arguments.
#. The status of co-simulation can be observed on the console.

.. _ariaid-title5:

Constraints for Co-simulation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are few limitations in performing co-simulation of the xfOpenCV
functions. They are:

#. Functions with multiple accelerators are not supported.
#. Compiler and simulator are default in HLS (gcc, xsim).
#. Since HLS does not support multi-kernel integration, the current flow
   also does not support multi-kernel integration. Hence, the Pyramidal
   Optical flow and Canny Edge Detection functions and examples are not
   supported in this flow.
#. The maximum image size (HEIGHT and WIDTH) set in config.h file should
   be equal to the actual input image size.

.. _ariaid-title6:

AXI Video Interface Functions
-----------------------------

xfOpenCV has functions that will transform the xf::Mat into Xilinx®
Video Streaming interface and vice-versa. ``xf::AXIvideo2xfMat()`` and
``xf::xfMat2AXIVideo()`` act as video interfaces to the IPs of the
xfOpenCV functions in the Vivado® IP integrator.
``cvMat2AXIvideoxf                 <NPC>`` and ``AXIvideo2cvMatxf<NPC>``
are used on the host side.

.. table:: Table 1. AXI Video Interface Functions

   +----------------------------+-----------------------------------------+
   | Video Library Function     | Description                             |
   +============================+=========================================+
   | AXIvideo2xfMat             | Converts data from an AXI4 video stream |
   |                            | representation to xf::Mat format.       |
   +----------------------------+-----------------------------------------+
   | xfMat2AXIvideo             | Converts data stored as xf::Mat format  |
   |                            | to an AXI4 video stream.                |
   +----------------------------+-----------------------------------------+
   | cvMat2AXIvideoxf           | Converts data stored as cv::Mat format  |
   |                            | to an AXI4 video stream                 |
   +----------------------------+-----------------------------------------+
   | AXIvideo2cvMatxf           | Converts data from an AXI4 video stream |
   |                            | representation to cv::Mat format.       |
   +----------------------------+-----------------------------------------+

.. _ariaid-title7:

AXIvideo2xfMat
~~~~~~~~~~~~~~

The ``AXIvideo2xfMat`` function receives a sequence of images using the
AXI4 Streaming Video and produces an ``xf::Mat`` representation.

API Syntax
^^^^^^^^^^

.. code:: pre

   template<int W,int T,int ROWS, int COLS,int NPC>
   int AXIvideo2xfMat(hls::stream< ap_axiu<W,1,1,1> >& AXI_video_strm, xf::Mat<T,ROWS, COLS, NPC>& img)

Parameter Descriptions
^^^^^^^^^^^^^^^^^^^^^^

The following table describes the template and the function parameters.

.. table:: Table 2. AXIvideo2cvMatxf Function Parameter Description

   +-----------------------------------+-----------------------------------+
   | Parameter                         | Description                       |
   +===================================+===================================+
   | W                                 | Data width of AXI4-Stream.        |
   |                                   | Recommended value is pixel depth. |
   +-----------------------------------+-----------------------------------+
   | T                                 | Pixel type of the image. 1        |
   |                                   | channel (XF_8UC1). Data width of  |
   |                                   | pixel must be no greater than W.  |
   +-----------------------------------+-----------------------------------+
   | ROWS                              | Maximum height of input image.    |
   +-----------------------------------+-----------------------------------+
   | COLS                              | Maximum width of input image.     |
   +-----------------------------------+-----------------------------------+
   | NPC                               | Number of pixels to be processed  |
   |                                   | per cycle. Possible options are   |
   |                                   | XF_NPPC1 and XF_NPPC8 for 1-pixel |
   |                                   | and 8-pixel operations            |
   |                                   | respectively.                     |
   +-----------------------------------+-----------------------------------+
   | AXI_video_strm                    | HLS stream of ap_axiu (axi        |
   |                                   | protocol) type.                   |
   +-----------------------------------+-----------------------------------+
   | img                               | Input image.                      |
   +-----------------------------------+-----------------------------------+

This function will return bit error of ERROR_IO_EOL_EARLY( 1 ) or
ERROR_IO_EOL_LATE( 2 ) to indicate an unexpected line length, by
detecting TLAST input.

For more information about AXI interface see UG761.

.. _ariaid-title8:

xfMat2AXIvideo
~~~~~~~~~~~~~~

The ``Mat2AXI`` video function receives an xf::Mat representation of a
sequence of images and encodes it correctly using the AXI4 Streaming
video protocol.

.. _api-syntax-1:

API Syntax
^^^^^^^^^^

.. code:: pre

   template<int W, int T, int ROWS, int COLS,int NPC>
   int xfMat2AXIvideo(xf::Mat<T,ROWS, COLS,NPC>& img,hls::stream<ap_axiu<W,1,1,1> >& AXI_video_strm)

.. _parameter-descriptions-1:

Parameter Descriptions
^^^^^^^^^^^^^^^^^^^^^^

The following table describes the template and the function parameters.

.. table:: Table 3. xfMat2AXIvideo Function Parameter Description

   +-----------------------------------+-----------------------------------+
   | Parameter                         | Description                       |
   +===================================+===================================+
   | W                                 | Data width of AXI4-Stream.        |
   |                                   | Recommended value is pixel depth. |
   +-----------------------------------+-----------------------------------+
   | T                                 | Pixel type of the image. 1        |
   |                                   | channel (XF_8UC1). Data width of  |
   |                                   | pixel must be no greater than W.  |
   +-----------------------------------+-----------------------------------+
   | ROWS                              | Maximum height of input image.    |
   +-----------------------------------+-----------------------------------+
   | COLS                              | Maximum width of input image.     |
   +-----------------------------------+-----------------------------------+
   | NPC                               | Number of pixels to be processed  |
   |                                   | per cycle. Possible options are   |
   |                                   | XF_NPPC1 and XF_NPPC8 for 1-pixel |
   |                                   | and 8-pixel operations            |
   |                                   | respectively.                     |
   +-----------------------------------+-----------------------------------+
   | AXI_video_strm                    | HLS stream of ap_axiu (axi        |
   |                                   | protocol) type.                   |
   +-----------------------------------+-----------------------------------+
   | img                               | Output image.                     |
   +-----------------------------------+-----------------------------------+

This function returns the value 0.

Note: The NPC values across all the functions in a data flow must follow
the same value. If there is mismatch it throws a compilation error in
HLS.

.. _ariaid-title9:

cvMat2AXIvideoxf
~~~~~~~~~~~~~~~~

The ``cvMat2Axivideoxf`` function receives image as cv::Mat
representation and produces the AXI4 streaming video of image.

.. _api-syntax-2:

API Syntax
^^^^^^^^^^

.. code:: pre

   template<int NPC,int W>
   void cvMat2AXIvideoxf(cv::Mat& cv_mat, hls::stream<ap_axiu<W,1,1,1> >& AXI_video_strm)

.. _parameter-descriptions-2:

Parameter Descriptions
^^^^^^^^^^^^^^^^^^^^^^

The following table describes the template and the function parameters.

.. table:: Table 4. AXIvideo2cvMatxf Function Parameter Description

   +-----------------------------------+-----------------------------------+
   | Parameter                         | Description                       |
   +===================================+===================================+
   | W                                 | Data width of AXI4-Stream.        |
   |                                   | Recommended value is pixel depth. |
   +-----------------------------------+-----------------------------------+
   | NPC                               | Number of pixels to be processed  |
   |                                   | per cycle. Possible options are   |
   |                                   | XF_NPPC1 and XF_NPPC8 for 1-pixel |
   |                                   | and 8-pixel operations            |
   |                                   | respectively.                     |
   +-----------------------------------+-----------------------------------+
   | AXI_video_strm                    | HLS stream of ap_axiu (axi        |
   |                                   | protocol) type.                   |
   +-----------------------------------+-----------------------------------+
   | cv_mat                            | Input image.                      |
   +-----------------------------------+-----------------------------------+

.. _ariaid-title10:

AXIvideo2cvMatxf
~~~~~~~~~~~~~~~~

The ``Axivideo2cvMatxf`` function receives image as AXI4 streaming video
and produces the cv::Mat representation of image

.. _api-syntax-3:

API Syntax
^^^^^^^^^^

.. code:: pre

   template<int NPC,int W>
   void AXIvideo2cvMatxf(hls::stream<ap_axiu<W,1,1,1> >& AXI_video_strm, cv::Mat& cv_mat) 

.. _parameter-descriptions-3:

Parameter Descriptions
^^^^^^^^^^^^^^^^^^^^^^

The following table describes the template and the function parameters.

.. table:: Table 5. AXIvideo2cvMatxf Function Parameter Description

   +-----------------------------------+-----------------------------------+
   | Parameter                         | Description                       |
   +===================================+===================================+
   | W                                 | Data width of AXI4-Stream.        |
   |                                   | Recommended value is pixel depth. |
   +-----------------------------------+-----------------------------------+
   | NPC                               | Number of pixels to be processed  |
   |                                   | per cycle. Possible options are   |
   |                                   | XF_NPPC1 and XF_NPPC8 for 1-pixel |
   |                                   | and 8-pixel operations            |
   |                                   | respectively.                     |
   +-----------------------------------+-----------------------------------+
   | AXI_video_strm                    | HLS stream of ap_axiu (axi        |
   |                                   | protocol) type.                   |
   +-----------------------------------+-----------------------------------+
   | cv_mat                            | Output image.                     |
   +-----------------------------------+-----------------------------------+