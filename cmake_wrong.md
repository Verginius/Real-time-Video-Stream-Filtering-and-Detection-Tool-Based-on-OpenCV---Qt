In file included from In file included from D:/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/cuda_runtime_api.h:148,                                                               
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimeBase.h:24,                                                                                   
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferLegacyDims.h:22,                                                                                    
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:21,                                                                                              
                 from E:\projects\RVSFDT\src\core\Detection\TensorRTBackend.h:6,                                                                                                     
                 from E:\projects\RVSFDT\src\core\Detection\DetectionBackend.cpp:5:                                                                                                  
D:/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/crt/host_defines.h:107: warning: "__cdecl" redefined                                                                              
  107 | #define __cdecl
      | 
<built-in>: note: this is the location of the previous definition                                                                                                                    
D:/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/cuda_runtime_api.h:148,                                                                                                           
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimeBase.h:24,                                                                                   
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferLegacyDims.h:22,                                                                                    
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:21,                                                                                              
                 from E:\projects\RVSFDT\src\core\Detection\TensorRTBackend.h:6,                                                                                                     
                 from E:\projects\RVSFDT\src\core\Detection\TensorRTBackend.cpp:2:                                                                                                   
D:/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/crt/host_defines.h:107: warning: "__cdecl" redefined                                                                              
  107 | #define __cdecl
      | 
<built-in>: note: this is the location of the previous definition                                                                                                                    
In file included from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimeCommon.h:34,                                                                                 
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:22,                                                                                          
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:27,                                                                                       
                 from E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:22:                                                                                              
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h: In member function 'virtual int32_t nvinfer1::IPluginV2Ext::getTensorRTVersion() const':                 
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:694:75: warning: 'nvinfer1::PluginVersion::kV2_EXT' is deprecated [-Wdeprecated-declarations]             
  694 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::kV2_EXT) << 24U)                                                                                   
      |                                                                           ^~~~~~~                                                                                            
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:99:5: note: declared here                                                                                 
   99 |     kV2_EXT TRT_DEPRECATED_ENUM = 1,                                                                                                                                         
      |     ^~~~~~~                                                                                                                                                                  
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:694:75: warning: 'nvinfer1::PluginVersion::kV2_EXT' is deprecated [-Wdeprecated-declarations]             
  694 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::kV2_EXT) << 24U)                                                                                   
      |                                                                           ^~~~~~~                                                                                            
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:99:5: note: declared here                                                                                 
   99 |     kV2_EXT TRT_DEPRECATED_ENUM = 1,                                                                                                                                         
      |     ^~~~~~~                                                                                                                                                                  
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h: In member function 'virtual int32_t nvinfer1::IPluginV2IOExt::getTensorRTVersion() const':               
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:813:75: warning: 'nvinfer1::PluginVersion::kV2_IOEXT' is deprecated [-Wdeprecated-declarations]           
  813 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::kV2_IOEXT) << 24U)                                                                                 
      |                                                                           ^~~~~~~~~                                                                                          
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:101:5: note: declared here                                                                                
  101 |     In file included from kV2_IOEXTE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimeCommon.h:34 TRT_DEPRECATED_ENUM = 2,                                    
      |     ,                                                                                                                                                                        
                      ^~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:22                                                                                  
,                                                                                                                                                                                    
                 from                                                                                                                                                           ,    
                 from                                                                           :                                                                                    
                                                                                                                               In member function '-Wdeprecated-declarationsvirtual int32_t nvinfer1::IPluginV2Ext::]                                                                                                                                                     
  813 |         return static_c                                                   getTensorRTVersionkV2_IOEXT() const) << 24U)                                                       
      |                                                                           ':                                                                                                 
^~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:694:75:                                                                                          
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:101:5:warning:  'note: nvinfer1::PluginVersion::kV2_EXTdeclared here                                     
  101 |     ' is deprecated [kV2_IOEXT-Wdeprecated-declarations TRT_DEPRECATED_ENUM = 2,                                                                                             
      |     ]                                                                                                                                                                        
  694 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::^~~~~~~~~kV2_EXT                                                                                   
) << 24U)                                                                                                                                                                            
      |                                                                           ^~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:                 
 At global scope:                                                                                                                                                                    
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:895:104:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:99:5:  warning: note: declared here
   99 |     IPluginV2kV2_EXT' is deprecated [ TRT_DEPRECATED_ENUM = 1,                                                                                                               
      |     -Wdeprecated-declarations^~~~~~~]                                                                                                                                        
  895 |     virtual IPluginV2* createPlugin(AsciiChar const* name, PluginFieldCollection const* fc) noexcept =                                                                       
                                                                                   ;                                                                                                 
      |                                                                                                                                                                              
'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:nvinfer1::PluginVersion::kV2_EXT ' is deprecated [note: -Wdeprecated-declarationsdeclared here    
  138 | class TRT_DEPRECATED ]
  694 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::IPluginV2kV2_EXT                                                                                   
      |                      ) << 24U)                                                                                                                                               
      |                                                                                                                                                                              
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:913:11:                                                                                  warning: note: 'declared here
   99 |                                       TRT_DEPRECATED_ENUM = 1,                                                                                                               
      |                                     ]                                                                                                                                        
  913 |         =                                                                                                                                                                    
0;                                                                                                                                                                                   
      |           ^                                                                                                                                                                  
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22: In member function ' virtual int32_t nvinfer1::IPluginV2IOExt::                        declared here                                                                                                        
  138 | class TRT_DEPRECATED () constIPluginV2':                                                                                                                                     
                                                                                                                                                                                     
                             E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:813:75:^~~~~~~~~                                                             
warning: 'nvinfer1::PluginVersion::kV2_IOEXT' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:976:31:-Wdeprecated-declarations ]           
  813 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::warning: kV2_IOEXT') << 24U)                                                                       
      |                                                                           IPluginCreator^~~~~~~~~' is deprecated [                                                           
                                                                                                          ]                                                                          
  976 | using IPluginCreator = v_1_0::               note: ;                                                                                                                         
      |                               declared here                                                                                                                                  
  101 |     ^~~~~~~~~~~~~~kV2_IOEXT                                                                                                                                                  
 TRT_DEPRECATED_ENUM = 2,                                                                                                                                                            
      |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:839:22:^~~~~~~~~                                                                              
note: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:813:75:declared here                                                                                
  839 | class TRT_DEPRECATED  IPluginCreatorwarning:  : public IPluginCreatorInterface                                                                                               
      |                       ^~~~~~~~~~~~~~nvinfer1::PluginVersion::kV2_IOEXT                                                                                                       
' is deprecated [                                                                                                            ]                                                       
  813 |         return static_cast<int32_t>((static_cast<uint32_t>(PluginVersion::warning:           ) << 24U)                                                                       
      |                                                                           IPluginCreator^~~~~~~~~' is deprecated [                                                           
-Wdeprecated-declarations]                                                                                                                                                           
  976 | using IPluginCreator = v_1_0::IPluginCreatorE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:101:5:;                                               
      |                                             note: ^declared here                                                                                                             
  101 |                                                                                                                                                                              
kV2_IOEXTE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:839:22: TRT_DEPRECATED_ENUM = 2,                                                                 
      |      ^~~~~~~~~note:                                                                                                                                                          
declared here                                                                                                                                                                        
  839 | class TRT_DEPRECATED IPluginCreator : public IPluginCreatorInterface                                                                                                         
      |                      ^~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:                                                               
 At global scope:
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:895:104: warning: 'IPluginV2' is deprecated [-Wdeprecated-declarations]                                   
  895 |     virtual IPluginV2* createPlugin(AsciiChar const* name, PluginFieldCollection const* fc) noexcept = 0;                                                                    
      |                                                                                                        E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:712:47:^                                                                                                                                                                                 
warning: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:'                                 declared here                                           
  138 | class TRT_DEPRECATED                                   ]                                                                                                                     
  712 |     virtual IPluginV2& getPlugin() noexcept =                                                                                                                                
      |                      0^~~~~~~~~;                                                                                                                                             
      |                                                                                                                                                                              
^E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:913:11:                                                                                                  
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:warning:         IPluginV2declared here                                                           
  138 | class TRT_DEPRECATED                  IPluginV2-Wdeprecated-declarations                                                                                                     
      |                      ]                                                                                                                                                       
  913 |         = ^~~~~~~~~0                                                                                                                                                         
;                                                                                                                                                                                    
      |           ^                                                                                                                                                                  
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22: note: declared here                                                                               
  138 | class TRT_DEPRECATED IPluginV2                                                                                                                                               
      |                                                                                                                                                                              
warning: 'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:976:31:IPluginV2                  warning:                          ']                          
 1210 |     virtual IPluginV2Layer* addPluginV2(ITensor* const* inputs, int32_t nbInputs, IPluginV2& plugin) noexcept = IPluginCreator0' is deprecated [;                            
      |                                                                                                                 -Wdeprecated-declarations^]                                  
  976 | using IPluginCreator = v_1_0::                                                                                                                                               
IPluginCreator;                                                                                                                                                                      
      |                               E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:^~~~~~~~~~~~~~                                               
note: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:839:22:declared here                                                                                
  138 | class TRT_DEPRECATED  IPluginV2note:                                                                                                                                         
      |                      declared here                                                                                                                                           
  839 | class TRT_DEPRECATED ^~~~~~~~~IPluginCreator                                                                                                                                 
 : public IPluginCreatorInterface                                                                                                                                                    
      |                      ^~~~~~~~~~~~~~                                                                                                                                          
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:976:45: warning: 'IPluginCreator' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:-Wdeprecated-declarations In member function ']                                                                                                                          
  976 | using IPluginCreator = v_1_0::IPluginCreatorvirtual int32_t nvinfer1::IPluginV2DynamicExt::;getTensorRTVersion                                                               
      |                                             () const^':                                                                                                                      
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:555:53:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:839:22:  warning: note: 'declared here
  839 | class TRT_DEPRECATED nvinfer1::PluginVersion::kV2_DYNAMICEXTIPluginCreator' is deprecated [ : public IPluginCreatorInterface                                                 
      |                      -Wdeprecated-declarations]                                                                                                                              
  555 |         return (static_cast<int32_t>(PluginVersion::^~~~~~~~~~~~~~kV2_DYNAMICEXT                                                                                             
) << 24 | (NV_TENSORRT_VERSION & 0xFFFFFF));                                                                                                                                         
      |                                                     ^~~~~~~~~~~~~~                                                                                                           
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:103:5: note: declared here                                                                                
  103 |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:712:47: warning: 'IPluginV2                 -Wdeprecated-declarations]                                 
  712 |     virtual IPluginV2& getPlugin() noexcept =                ;                                                                                                               
      |                                                TRT_DEPRECATED_ENUM = 3,                                                                                                      
      |     ^                                                                                                                                                                        
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22: note: declared here                                                                               
  138 | class TRT_DEPRECATED IPluginV2                                                                                                                                               
      |                      ^~~~~~~~~                                                                                                                                               
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:555:53: warning: 'nvinfer1::PluginVersion::kV2_DYNAMICEXT' is deprecated [-Wdeprecated-declarations]            
  555 |         return (static_cast<int32_t>(PluginVersion::kV2_DYNAMICEXT) << 24 | (NV_TENSORRT_VERSION & 0xFFFFFF));                                                               
      |                                                     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1210:113: warning: 'IPluginV2^~~~~~~~~~~~~~' is deprecated [-Wdeprecated-declarations]                                                                                                                                                      
 1210 |     virtual IPluginV2Layer* addPluginV2(ITensor* const* inputs, int32_t nbInputs, IPluginV2& plugin) noexcept = 0;                                                           
      |                                                                                                                 
^                                                                                                                                                                                    
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22: note: declared here                                                                               
  138 | class TRT_DEPRECATED E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:103:5:IPluginV2                                                              
      |                       ^~~~~~~~~                                                                                                                                              
note: declared here                                                                                                                                                                  
  103 |     kV2_DYNAMICEXT TRT_DEPRECATED_ENUM = 3,                                                                                                                                  
      |     ^~~~~~~~~~~~~~                                                                                                                                                           
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h: In member function 'virtual int32_t nvinfer1::IPluginV2DynamicExt::                  () constE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:':                                                                                                                                
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:555:53:  In member function 'warning: 'nvinfer1::PluginVersion::kV2_DYNAMICEXT' is deprecated [virtual void* nvinfer1::v_1_0::IGpuAllocator::-Wdeprecated-declarations]                                                                                                                              
  555 |         return (static_cast<int32_t>(PluginVersion::kV2_DYNAMICEXT) << 24 | (NV_TENSORRT_VERSION & 0xFFFFFF));                                                               
      |                                                     allocateAsync(uint64_t, uint64_t, nvinfer1::AllocatorFlags, cudaStream_t)^~~~~~~~~~~~~~':                                

E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1781:24:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:103:5:  warning: note: 'declared here
  103 |                                                   kV2_DYNAMICEXTallocate TRT_DEPRECATED_ENUM = 3,                                                                            
      |     (uint64_t, uint64_t, nvinfer1::AllocatorFlags)^~~~~~~~~~~~~~' is deprecated [                                                                                            
-Wdeprecated-declarationsE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:555:53:]                                                                               
 1781 |         return  allocate(size, alignment, flags)warning: ;                                                                                                                   
      |                '                                nvinfer1::PluginVersion::kV2_DYNAMICEXT                                                                                      
' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1682:34:                          ]                                                            
  555 |         return (static_cast<int32_t>(PluginVersion::      kV2_DYNAMICEXTdeclared here                                                                                        
 1682 |     TRT_DEPRECATED virtual void* ) << 24 | (NV_TENSORRT_VERSION & 0xFFFFFF));                                                                                                
      |                                                     allocate^~~~~~~~~~~~~~(                                                                                                  
      |                                                                                                                                                                              
^~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:103:5:                                                                                            
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:note:  In member function 'declared here                                                                       
  103 |                                                  kV2_DYNAMICEXTdeallocateAsync TRT_DEPRECATED_ENUM = 3,                                                                      
      |     (void*, cudaStream_t)^~~~~~~~~~~~~~':                                                                                                                                    
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1813:26: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:          In member function ''virtual void* nvinfer1::v_1_0::IGpuAllocator::virtual bool nvinfer1::v_1_0::IGpuAllocator::allocateAsyncdeallocate(uint64_t, uint64_t, nvinfer1::AllocatorFlags, cudaStream_t)(void*)': 
' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1781:24:-Wdeprecated-declarations ]                                                            
 1813 |         return warning: deallocate(memory)';                                                                                                                                 
      |                virtual void* nvinfer1::v_1_0::IGpuAllocator::~~~~~~~~~~^~~~~~~~allocate                                                                                      
(uint64_t, uint64_t, nvinfer1::AllocatorFlags)                                                                                                                              ]        
 1781 |         return declared here                                                                                                                                                 
 1748 |     TRT_DEPRECATED virtual bool                                           ;                                                                                                  
      |                (void* const memory) noexcept = 0;                                                                                                                            
      |                                                                                                                                                                              
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1682:34: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:note:  In member function 'declared here
 1682 |     TRT_DEPRECATED virtual void*                                                  allocatereallocateOutputAsync(                                                             
      |                                  (const char*, void*, uint64_t, uint64_t, cudaStream_t)^~~~~~~~':                                                                            
                                                                                                                                                                                     
                                                                             E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:  In member function 'warning: virtual bool nvinfer1::v_1_0::IGpuAllocator:: deallocateAsyncvirtual void* nvinfer1::v_1_0::IOutputAllocator::(void*, cudaStream_t)reallocateOutput':                                    
                                                                                                                                                                         ]           
 4242 |         return 'reallocateOutput(tensorName, currentMemory, size, alignment)virtual bool nvinfer1::v_1_0::IGpuAllocator::;                                                   
      |                deallocate~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~(void*)                                                                                 
' is deprecated [-Wdeprecated-declarationsE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:4211:34:]                                                             
 1813 |         return  deallocate(memory)note: ;                                                                                                                                    
      |                declared here                                                                                                                                                 
 4211 |     TRT_DEPRECATED virtual void* ~~~~~~~~~~^~~~~~~~reallocateOutput                                                                                                          
(                                                                                                                                                                                    
      |                                  E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:1748:33:^~~~~~~~~~~~~~~~                                               
note: declared here                                                                                                                                                                  
 1748 |     TRT_DEPRECATED virtual bool deallocate(void* const memory) noexcept = 0;                                                                                                 
      |                                 ^~~~~~~~~~                                                                                                                                   
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:                                                                      At global scope:                                 
                     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:2780:28:                                                  reallocateOutputAsyncwarning: (const char*, void*, uint64_t, uint64_t, cudaStream_t)'':                                                                                                                                      
IPluginV2E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:4242:32:' is deprecated [ -Wdeprecated-declarationswarning: ]                                          
 2780 |     IPluginV2& getPlugin() 'noexceptvirtual void* nvinfer1::v_1_0::IOutputAllocator::                                                                                        
      |                            reallocateOutput^~~~~~~~(const char*, void*, uint64_t, uint64_t)                                                                                  
' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:-Wdeprecated-declarations ]                                                       
 4242 |         return note: reallocateOutput(tensorName, currentMemory, size, alignment)declared here                                                                               
  138 | class TRT_DEPRECATED ;                                                                                                                                                       
      |                IPluginV2~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                                                                         
      |                                                                                                                                                                              
^~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntime.h:4211:34:                                                                                               
 note: declared here                                                                                                                                                                 
 4211 |     TRT_DEPRECATED virtual void* reallocateOutput(                                                                                                                           
      |                                  E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:^~~~~~~~~~~~~~~~ In member function '                                         
void nvinfer1::IDynamicQuantizeLayer::setAxis(int32_t)':                                                                                                                             
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5922:23: warning: 'virtual void nvinfer1::apiv::VDynamicQuantizeLayer::setAxis(int32_t)' is deprecated [-Wdeprecated-declarations]                                                                                                                                                                          
 5922 |         mImpl->setAxis(axis);                                                                                                                                                
      |         ~~~~~~~~~~~~~~^~~~~~                                                                                                                                                 
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1036:33: note: declared here                                                                                       
 1036 |     TRT_DEPRECATED virtual void setAxis(int32_t axis) noexcept = 0;                                                                                                          
      |                                 ^~~~~~~                                                                                                                                      
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h: At global scope:                                                                                                      
                                                                                                                                                          int32_t nvinfer1::IDynamicQuantizeLayer::                 () constIPluginV2':                                                                                                                                   
' is deprecated [                                                                                                ]                                                                   
 2780 |     IPluginV2& getPlugin() warning: noexcept'                                                                                                                                
      |                            virtual int32_t nvinfer1::apiv::VDynamicQuantizeLayer::                                                                                           
() const' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferRuntimePlugin.h:138:22:-Wdeprecated-declarations ]                                               
 5932 |         return note: mImpl->getAxis()declared here                                                                                                                           
  138 | class TRT_DEPRECATED ;                                                                                                                                                       
      |                                                                                                                                                                              
      |                                                                                                                                                                              
^~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1035:36:                                                                                                  
 note: declared here                                                                                                                                                                 
 1035 |     TRT_DEPRECATED virtual int32_t getAxis() const noexcept = 0;                                                                                                             
      |                                    ^~~~~~~                                                                                                                                   
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h: In member function 'void nvinfer1::IDynamicQuantizeLayer::setBlockSize(int32_t)':                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5945:28: warning: 'virtual void nvinfer1::apiv::VDynamicQuantizeLayer::setBlockSize(int32_t)' is deprecated [-Wdeprecated-declarationsE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:]                                                                                                       
 5945 |                              mImpl->setBlockSize(size)void nvinfer1::IDynamicQuantizeLayer::;                                                                                
      |         setAxis~~~~~~~~~~~~~~~~~~~^~~~~~(int32_t)                                                                                                                            
':                                                                                                                                                                                   
                                                                          E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5922:23:  note: warning: declared here       
 1038 |     TRT_DEPRECATED virtual void  setBlockSizevirtual void nvinfer1::apiv::VDynamicQuantizeLayer::(int32_t axis) noexcept = 0;                                                
      |                                 setAxis^~~~~~~~~~~~(int32_t)                                                                                                                 
' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:-Wdeprecated-declarations In member function ']                                                       
 5922 |         int32_t nvinfer1::IDynamicQuantizeLayer::mImpl->setAxis(axis);                                                                                                       
      |         getBlockSize~~~~~~~~~~~~~~^~~~~~() const                                                                                                                             
':
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5955:35:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1036:33:  warning: note: 'declared here      
 1036 |     TRT_DEPRECATED virtual void virtual int32_t nvinfer1::apiv::VDynamicQuantizeLayer::setAxisgetBlockSize(int32_t axis) noexcept = 0;                                       
      |                                         ^~~~~~~' is deprecated [                                                                                                             
-Wdeprecated-declarations]                                                                                                                                                           
 5955 |         return mImpl->getBlockSize()E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:;                                                                          
      |                 In member function '~~~~~~~~~~~~~~~~~~~^~int32_t nvinfer1::IDynamicQuantizeLayer::                                                                           
getAxis() constE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1037:36:':                                                                                          
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5932:30:note:  declared here                                                                                          
 1037 |     TRT_DEPRECATED virtual int32_t warning: getBlockSize'() const noexcept = 0;                                                                                              
      |                                    virtual int32_t nvinfer1::apiv::VDynamicQuantizeLayer::^~~~~~~~~~~~getAxis                                                                
() const' is deprecated [-Wdeprecated-declarations]                                                                                                                                  
 5932 |         return mImpl->getAxis();                                                                                                                                             
      |                ~~~~~~~~~~~~~~^~                                                                                                                                              
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1035:36: note: declared here                                                                                       
 1035 |     TRT_DEPRECATED virtual int32_t getAxis() const noexcept = 0;                                                                                                             
      |                                    ^~~~~~~                                                                                                                                   
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h: In member function 'void nvinfer1::IDynamicQuantizeLayer::setBlockSize(int32_t)':                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:5945:28: warning: 'virtual void nvinfer1::apiv::VDynamicQuantizeLayer::setBlockSize(int32_t)' is deprecated [-Wdeprecated-declarations]                                                                                                                                                                     
 5945 |         mImpl->setBlockSize(size);                                                                                                                                           
      |         ~~~~~~~~~~~~~~~~~~~^~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:                                                                              
 In member function 'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1038:33:virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8EntropyCalibrator:: getAlgorithmnote: ()declared here                                                                                                                                                          
 1038 |     TRT_DEPRECATED virtual void ':
setBlockSizeE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9912:37:(int32_t axis) noexcept = 0;                                                                       
      |                                  ^~~~~~~~~~~~warning:                                                                                                                        
'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION In member function '' is deprecated [int32_t nvinfer1::IDynamicQuantizeLayer::-Wdeprecated-declarationsgetBlockSize]                                                                                                                                   
 9912 |         return CalibrationAlgoType::                            ':                                                                                                           
;                                                                                                                                                                                    
      |                                                                                                                                                                              
warning: 'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9801:5: virtual int32_t nvinfer1::apiv::VDynamicQuantizeLayer::note: getBlockSizedeclared here               
 9801 |     () constkENTROPY_CALIBRATION' is deprecated [ TRT_DEPRECATED_ENUM = 1,   //!< Legacy entropy calibration                                                                 
      |                                                  ]                                                                                                                           
 5955 |         return                                                                                                                                                               
mImpl->getBlockSize()E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9912:37:;                                                                                         
      |                                                                                                                                                                              
'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInferImpl.h:1037:36:nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION ' is deprecated [note: -Wdeprecated-declarationsdeclared here
 1037 |     TRT_DEPRECATED virtual int32_t ]                                                                                                                                         
 9912 |         return CalibrationAlgoType::getBlockSizekENTROPY_CALIBRATION() const noexcept = 0;                                                                                   
      |                                    ;                                                                                                                                         
      |                                     ^~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~                                                                                                         
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9801:5: note: declared here                                                                                            
 9801 |     kENTROPY_CALIBRATION TRT_DEPRECATED_ENUM = 1,   //!< Legacy entropy calibration                                                                                          
      |     ^~~~~~~~~~~~~~~~~~~~                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h: At global scope:                                                                                                      
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9932:39: warning: 'IInt8EntropyCalibrator' is deprecated [-Wdeprecated-declarations]                                   
 9932 | using IInt8EntropyCalibrator = v_1_0::IInt8EntropyCalibrator;                                                                                                                
      |                                       ^~~~~~~~~~~~~~~~~~~~~~                                                                                                                 
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9896:22: note: declared here                                                                                           
 9896 | class TRT_DEPRECATED IInt8EntropyCalibrator : public IInt8Calibrator                                                                                                         
      |                      ^~~~~~~~~~~~~~~~~~~~~~                                                                                                                                  
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9932:61: warning: 'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:IInt8EntropyCalibrator In member function '                 virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8EntropyCalibrator::                                     ]                                          
 9932 | using IInt8EntropyCalibrator = v_1_0::IInt8EntropyCalibrator();':                                                                                                            
                                                                                                                                                                                     
                                                                    E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9912:37:^                                          
warning:  E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9896:22:nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION                  note:                          declared here
 9896 | class TRT_DEPRECATED ]                                                                                                                                                       
 9912 |         return Calibr               IInt8EntropyCalibratorkENTROPY_CALIBRATION : public IInt8Calibrator                                                                      
      |                      ;                                                                                                                                                       
      |                                     ^~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~                                                                                               
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9801:5:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:  In member function 'note: virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8EntropyCalibrator2::declared here                                                                                                              
 9801 |     getAlgorithm()kENTROPY_CALIBRATION':                                                                                                                                     
 TRT_DEPRECATED_ENUM = 1,   //!< Legacy entropy calibration                                                                                                                          
      |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9952:37:^~~~~~~~~~~~~~~~~~~~                                                                               
warning: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9912:37:' nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION_2warning: ' is deprecated ['-Wdeprecated-declarationsnvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION]                                                                                                                            
 9952 |         return CalibrationAlgoType::' is deprecated [kENTROPY_CALIBRATION_2-Wdeprecated-declarations;                                                                        
      |                                     ]                                                                                                                                        
 9912 |         return CalibrationAlgoType::^~~~~~~~~~~~~~~~~~~~~~kENTROPY_CALIBRATION                                                                                               
;                                                                                                                                                                                    
      |                                     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9802:5:^~~~~~~~~~~~~~~~~~~~                                                
note: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9801:5:declared here                                                                                             
 9802 |      kENTROPY_CALIBRATION_2note:  TRT_DEPRECATED_ENUM = 2, //!< Entropy calibration                                                                                          
      |     declared here                                                                                                                                                            
 9801 |     ^~~~~~~~~~~~~~~~~~~~~~kENTROPY_CALIBRATION                                                                                                                               
 TRT_DEPRECATED_ENUM = 1,   //!< Legacy entropy calibration                                                                                                                          
      |                                                                                                                                                                              
          E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:' At global scope:                                                                                           
nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION_2E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9932:39:' is deprecated [ warning: -Wdeprecated-declarations']    
 9952 |         return CalibrationAlgoType::                      kENTROPY_CALIBRATION_2' is deprecated [;                                                                           
      |                                     -Wdeprecated-declarations^~~~~~~~~~~~~~~~~~~~~~]                                                                                         
 9932 | using IInt8EntropyCalibrator = v_1_0::                                                                                                                                       
IInt8EntropyCalibrator;                                                                                                                                                              
      |                                       E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9802:5:^~~~~~~~~~~~~~~~~~~~~~                                            
note: declared here                                                                                                                                                                  
 9802 |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9896:22:kENTROPY_CALIBRATION_2  TRT_DEPRECATED_ENUM = 2, //!< Entropy calibration                          
      |     note: ^~~~~~~~~~~~~~~~~~~~~~declared here                                                                                                                                
 9896 | class TRT_DEPRECATED                                                                                                                                                         
IInt8EntropyCalibrator : public IInt8Calibrator                                                                                                                                      
      |                                                                                                           At global scope:                                                   
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9972:40:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9932:61:  warning: warning: ' IInt8EntropyCalibrator2IInt8EntropyCalibrator' is deprecated [' is deprecated [-Wdeprecated-declarations-Wdeprecated-declarations]                                                                      
 9972 | using IInt8EntropyCalibrator2 = v_1_0::]                                                                                                                                     
 9932 | using IInt8EntropyCalibrator = v_1_0::I                     IInt8EntropyCalibrator2;;                                                                                        
      |                                                                                                                                                                              
      |                                                             ^~~~~~~~~~~~~~~~~~~~~~~^                                                                                         
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9936:22:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9896:22:        note: declared here              
 9936 | class TRT_DEPRECATED declared here                                                                                                                                           
 9896 | class TRT_DEPRECATED IInt8EntropyCalibrator2IInt8EntropyCalibrator : public IInt8Calibrator                                                                                  
      |                       : public IInt8Calibrator                                                                                                                               
      |                      ^~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~                                                                                                           
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9972:63: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:warning:  In member function ''virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8EntropyCalibrator2::IInt8EntropyCalibrator2getAlgorithm' is deprecated [                           ':                                     
]                                                                                                                                                                                    
                                                                      E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9952:37:;                                        
      |                                                               warning: ^'                                                                                                    
nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION_2' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9936:22:-Wdeprecated-declarations ]              
 9952 |         return CalibrationAlgoType::                            declared here                                                                                                
 9936 | class TRT_DEPRECATED ;                                                                                                                                                       
      |                                     IInt8EntropyCalibrator2^~~~~~~~~~~~~~~~~~~~~~ : public IInt8Calibrator                                                                   
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~~~                                                                                                                                                              
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9802:5: note: declared here                                                                                            
 9802 |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h: In member function 'kENTROPY_CALIBRATION_2 TRT_DEPRECATED_ENUM = 2, //!< Entropy calibration              
      |     virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8MinMaxCalibrator::^~~~~~~~~~~~~~~~~~~~~~                                                                     
()':                                                                                                                                                                                 
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9952:37:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9992:37:  warning: warning: ' nvinfer1::CalibrationAlgoType::kENTROPY_CALIBRATION_2                                                                   ' is deprecated [                         -Wdeprecated-declarations]            
 9952 |         return CalibrationAlgoType::]                                                                                                                                        
 9992 |         return CalibrationAlgoType::kENTROPY_CALIBRATION_2kMINMAX_CALIBRATION;                                                                                               
      |                                     ;                                                                                                                                        
      |                                     ^~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~                                                                                                
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9802:5:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9803:5:  note: note: declared here                
 9802 |     declared here                                                                                                                                                            
 9803 |     kENTROPY_CALIBRATION_2kMINMAX_CALIBRATION TRT_DEPRECATED_ENUM = 2, //!< Entropy calibration                                                                              
      |      TRT_DEPRECATED_ENUM = 3,    //!< Minmax calibration                                                                                                                     
      |                                                                                                                                                                              
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9992:37: At global scope:                                
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9972:40:warning:  'warning: nvinfer1::CalibrationAlgoType::kMINMAX_CALIBRATION'' is deprecated [IInt8EntropyCalibrator2-Wdeprecated-declarations' is deprecated []                                                                                                                                         
 9992 |         return CalibrationAlgoType::                                            ]                                                                                            
 9972 | using IInt8EntropyCalibrator2 = v_1_0::;                                                                                                                                     
      |                                                            ^~~~~~~~~~~~~~~~~~~;                                                                                              
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9803:5:                                                                                         
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9936:22:note:  declared here                                                                                          
 9803 |     note: kMINMAX_CALIBRATIONdeclared here                                                                                                                                   
 9936 | class TRT_DEPRECATED  TRT_DEPRECATED_ENUM = 3,    //!< Minmax calibration                                                                                                    
      |     IInt8EntropyCalibrator2^~~~~~~~~~~~~~~~~~~ : public IInt8Calibrator                                                                                                      
      |                      
^~~~~~~~~~~~~~~~~~~~~~~                                                                                                                                                              
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9972:63: At global scope:                                
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10011:38: warning: 'warning: IInt8EntropyCalibrator2'                 IInt8MinMaxCalibrator                         ' is deprecated []                                                                                                                                                                     
 9972 | using IInt8EntropyCalibrator2 = v_1_0::IInt8EntropyCalibrator2                          ]                                                                                    
10011 | using IInt8MinMaxCalibrator = v_1_0::                                                                                                                                        
      |                                                                                     ;                                                                                        
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9936:22:                                                                                          
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9976:22:note:  declared here                                                                                          
 9936 | class TRT_DEPRECATED IInt8EntropyCalibrator2note:  : public IInt8Calibrator                                                                                                  
      |                      declared here                                                                                                                                           
 9976 | class TRT_DEPRECATED                        IInt8MinMaxCalibrator                                                                                                            
 : public IInt8Calibrator                                                                                                                                                            
      |                      ^~~~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:                                                                     
 In member function 'virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8MinMaxCalibrator::E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10011:59:getAlgorithm ()warning: ':                                                                                                                                                                      
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9992:37:IInt8MinMaxCalibrator                  warning: -Wdeprecated-declarations']                                   
10011 | using IInt8MinMaxCalibrator = v_1_0::IInt8MinMaxCalibratornvinfer1::CalibrationAlgoType::kMINMAX_CALIBRATION;' is deprecated [                                               
      |                                                           -Wdeprecated-declarations^]                                                                                        
 9992 |         return CalibrationAlgoType::                                                                                                                                         
kMINMAX_CALIBRATIONE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9976:22:;                                                                                           
      |                                      ^~~~~~~~~~~~~~~~~~~note:                                                                                                                
declared here                                                                                                                                                                        
 9976 | class TRT_DEPRECATED                                                                                             : public IInt8Calibrator                                    
      |                      note:                      declared here                                                                                                                
 9803 |                                                                                                                                                                              
kMINMAX_CALIBRATION                                                               TRT_DEPRECATED_ENUM = 3,    //!< Minmax calibration                                                
      |      In member function '^~~~~~~~~~~~~~~~~~~virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8LegacyCalibrator::                                                   
getAlgorithm()E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9992:37:':                                                                                               
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10031:37:warning:  'warning: nvinfer1::CalibrationAlgoType::kMINMAX_CALIBRATION'' is deprecated [nvinfer1::CalibrationAlgoType::kLEGACY_CALIBRATION-Wdeprecated-declarations' is deprecated []                                                                                                             
 9992 |         return CalibrationAlgoType::-Wdeprecated-declarationskMINMAX_CALIBRATION]                                                                                            
10031 |         return CalibrationAlgoType::;                                                                                                                                        
      |                                     kLEGACY_CALIBRATION^~~~~~~~~~~~~~~~~~~;                                                                                                  
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9803:5:                                                                                             
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9800:5:note:  declared here                                                                                           
 9803 |     note: kMINMAX_CALIBRATIONdeclared here                                                                                                                                   
 9800 |      TRT_DEPRECATED_ENUM = 3,    //!< Minmax calibration                                                                                                                     
      |     kLEGACY_CALIBRATION^~~~~~~~~~~~~~~~~~~ TRT_DEPRECATED_ENUM = 0,    //!< Legacy calibration                                                                               
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:                                                                                                    
 At global scope:                                                                                                                                                                    
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10031:37:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10011:38:  warning: warning: ''nvinfer1::CalibrationAlgoType::kLEGACY_CALIBRATION                                      ' is deprecated [                                                  ]                                          
10031 |         return CalibrationAlgoType::]                                                                                                                                        
10011 | using IInt8MinMaxCalibrator = v_1_0::kLEGACY_CALIBRATIONIInt8MinMaxCalibrator;                                                                                               
      |                                     ;                                                                                                                                        
      |                                      ^~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~                                                                                                
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9800:5:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9976:22:  note: note: declared here               
 9800 |     declared here                                                                                                                                                            
 9976 | class TRT_DEPRECATED kLEGACY_CALIBRATIONIInt8MinMaxCalibrator TRT_DEPRECATED_ENUM = 0,    //!< Legacy calibration                                                            
      |      : public IInt8Calibrator                                                                                                                                                
      |                                                                                                                                                                              
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10011:59: At global scope:                               
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10091:38:warning:  'warning: IInt8MinMaxCalibrator'                 IInt8LegacyCalibrator-Wdeprecated-declarations' is deprecated []                                                                                                                                                                       
10011 | using                                                     -Wdeprecated-declarations;]                                                                                        
10091 | using IInt8LegacyCalibrator = v_1_0::                                                                                                                                        
      |                                                           IInt8LegacyCalibrator ;                                                                                            
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9976:22:                                                                                          
                                                                               declared here                                                                                         
 9976 | class TRT_DEPRECATED                            declared here                                                                                                                
10015 | class TRT_DEPRECATED  : public IInt8Calibrator                                                                                                                               
      |                      IInt8LegacyCalibrator^~~~~~~~~~~~~~~~~~~~~ : public IInt8Calibrator                                                                                     
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~                                                                                                                                                                
                                                                                                                                       In member function '         virtual nvinfer1::CalibrationAlgoType nvinfer1::v_1_0::IInt8LegacyCalibrator::'getAlgorithmIInt8LegacyCalibrator()' is deprecated [':                                                                 
-Wdeprecated-declarationsE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10031:37:]                                                                                    
10091 | using IInt8LegacyCalibrator = v_1_0::IInt8LegacyCalibrator ;warning:                                                                                                         
      |                                                           '^                                                                                                                 
nvinfer1::CalibrationAlgoType::kLEGACY_CALIBRATION' is deprecated [E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10015:22:-Wdeprecated-declarations ]                
10031 |         return CalibrationAlgoType::note: kLEGACY_CALIBRATIONdeclared here                                                                                                   
10015 | class TRT_DEPRECATED ;                                                                                                                                                       
      |                                     IInt8LegacyCalibrator                    : public IInt8Calibrator                                                                        
      |                                                                                                                                                                              
^~~~~~~~~~~~~~~~~~~~~                                                                                                                                                                
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:9800:5: note: declared here                                                                                            
 9800 |     kLEGACY_CALIBRATIONE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10267:58: TRT_DEPRECATED_ENUM = 0,    //!< Legacy calibration                           
      |      ^~~~~~~~~~~~~~~~~~~warning:                                                                                                                                             
'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10031:37:IAlgorithmVariant ' is deprecated [warning: -Wdeprecated-declarations ]                                      
10267 |     IAlgorithmVariant const& getAlgorithmVariant() const                                                                                                                     
      |                                                          -Wdeprecated-declarations^~~~~~~~]                                                                                  
10031 |         return CalibrationAlgoType::
kLEGACY_CALIBRATIONE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10168:22:;                                                                                          
      |                                      ^~~~~~~~~~~~~~~~~~~note:                                                                                                                
declared here                                                                                                                                                                        
10168 | class TRT_DEPRECATED                                                                      IAlgorithmVariant  : public INoCopy                                                
      |                      note: ^~~~~~~~~~~~~~~~~declared here                                                                                                                    
 9800 |                                                                                                                                                                              
kLEGACY_CALIBRATION TRT_DEPRECATED_ENUM = 0,    //!< Legacy calibration                                                                                                              
      |     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10297:76:^~~~~~~~~~~~~~~~~~~                                                                               
warning: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:' At global scope:                                                                                            
IAlgorithmIOInfoE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10091:38:' is deprecated [ -Wdeprecated-declarationswarning: ]                                         
10297 |     IAlgorithmIOInfo const* getAlgorithmIOInfoByIndex(int32_t index) const  noexceptIInt8LegacyCalibrator                                                                    
      |                                                                                             ^~~~~~~~-Wdeprecated-declarations                                                
]                                                                                                                                                                                    
10091 | using IInt8LegacyCalibrator = v_1_0::E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10105:22:IInt8LegacyCalibrator ;                                          
      |                                      note: ^~~~~~~~~~~~~~~~~~~~~declared here                                                                                                
10105 | class TRT_DEPRECATED                                                                                                                                                         
IAlgorithmIOInfoE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10015:22: : public INoCopy                                                                             
      |                       ^~~~~~~~~~~~~~~~note:                                                                                                                                  
declared here                                                                                                                                                                        
10015 | class TRT_DEPRECATED E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10334:59:IInt8LegacyCalibrator  : public IInt8Calibrator                                  
      |                      warning: ^~~~~~~~~~~~~~~~~~~~~'                                                                                                                         
IAlgorithmContextE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10091:59:' is deprecated [ -Wdeprecated-declarationswarning: ]                                        
10334 |         int32_t nbChoices, int32_t* selection) noexcept = '0IInt8LegacyCalibrator;                                                                                           
      |                                                                            ^                                                                                                 
-Wdeprecated-declarations]                                                                                                                                                           
10091 | using IInt8LegacyCalibrator = v_1_0::IInt8LegacyCalibratorE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10202:22:;                                           
      |                                                           note:  declared here                                                                                               
10202 | class TRT_DEPRECATED                                                                                                                                                         
IAlgorithmContextE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10015:22: : public INoCopy                                                                            
      |                       ^~~~~~~~~~~~~~~~~note:                                                                                                                                 
declared here                                                                                                                                                                        
10015 | class TRT_DEPRECATED E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10334:59:IInt8LegacyCalibrator  : public IInt8Calibrator                                  
      |                      warning:                                                                                                                                                
IAlgorithm' is deprecated [-Wdeprecated-declarations]                                                                                                                                
10334 |         int32_t nbChoices, int32_t* selection) noexcept = 0E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10267:58:;                                          
      |                                                            ^warning:                                                                                                         
'IAlgorithmVariantE:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10261:22:' is deprecated [ -Wdeprecated-declarationsnote: ]                                          
10267 |     IAlgorithmVariant const& getAlgorithmVariant() const declared here                                                                                                       
10261 | class TRT_DEPRECATED noexceptIAlgorithm                                                                                                                                      
      |                                                           : public INoCopy                                                                                                   
      |                      ^~~~~~~~^~~~~~~~~~                                                                                                                                      
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10168:22: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10347:42:note:  declared here                  
10168 | class TRT_DEPRECATED warning:                    : public INoCopy                                                                                                            
      |                                                                                                                                                                              
-Wdeprecated-declarations]                                                                                                                                                           
10347 |         int32_t nbAlgorithms) noexcept = E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10297:76:0 ;                                                          
      |                                                   ^'                                                                                                                         
IAlgorithmIOInfo                                                                                         -Wdeprecated-declarationsnote: ]                                            
10297 |     IAlgorithmIOInfo const* getAlgorithmIOInfoByIndex(int32_t index) const declared here                                                                                     
10202 | class TRT_DEPRECATED noexceptIAlgorithmContext                                                                                                                               
      |                                                                             : public INoCopy                                                                                 
      |                      ^~~~~~~~^~~~~~~~~~~~~~~~~                                                                                                                               
                                                                                                                                                                                     
                                                                                                                                                note: warning: declared here         
10105 | class TRT_DEPRECATED '                           : public INoCopy                                                                                                            
      |                      ' is deprecated [^~~~~~~~~~~~~~~~-Wdeprecated-declarations                                                                                              
]                                                                                                                                                                                    
10347 |         int32_t nbAlgorithms) noexcept = 0;                                                                                                                                  
      |                                          ^                                                                                                                                   
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10334:59:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10261:22:  warning: note: 'declared here        
10261 | class TRT_DEPRECATED IAlgorithmContextIAlgorithm' is deprecated [ : public INoCopy                                                                                           
      |                      -Wdeprecated-declarations^~~~~~~~~~]                                                                                                                    
10334 |         int32_t nbChoices, int32_t* selection) noexcept =                                                                                                                    
0;                                                                                                                                                                                   
      |                                                           ^E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10365:35:                                           
 E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10202:22:warning:   note: IAlgorithmSelectordeclared here                                                             
10202 | class TRT_DEPRECATED                  IAlgorithmContext-Wdeprecated-declarations : public INoCopy                                                                            
      |                      ]                                                                                                                                                       
10365 | using IAlgorithmSelec             ^~~~~~~~~~~~~~~~~IAlgorithmSelector                                                                                                        
;                                                                                                                                                                                    
      |                                   E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10334:59:^~~~~~~~~~~~~~~~~~                                                  
warning: E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10309:22:' IAlgorithmnote: ' is deprecated [declared here                                                     
10309 | class TRT_DEPRECATED -Wdeprecated-declarations]                                                                                                                              
10334 |         int32_t nbChoices, int32_t* selection) noexcept = IAlgorithmSelector0 : public IVersionedInterface                                                                   
      |                      ;                                                                                                                                                       
      |                                                           ^~~~~~~~~~~~~~~~~~                                                                                                 
                                                                                                                                                                                     
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10365:53:E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10261:22:                  declared here        
10261 | class TRT_DEPRECATED                                               : public INoCopy                                                                                          
      |                                                         ]                                                                                                                    
10365 | using IAlgorithmSelector = v_1_0::IAlgorithmSelector                                                                                                                         
;                                                                                                                                                                                    
      |                                                     E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10347:42:^                                                 
warning: 'E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10309:22: IAlgorithmContextnote: ' is deprecated [declared here                                              
10309 | class TRT_DEPRECATED -Wdeprecated-declarationsIAlgorithmSelector]                                                                                                            
10347 |         int32_t nbAlgorithms) noexcept =  : public IVersionedInterface                                                                                                       
      |                      0^~~~~~~~~~~~~~~~~~;                                                                                                                                    
      |                                                                                                                                                                              
^                                                                                                                                                                                    
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10202:22: note: declared here                                                                                          
10202 | class TRT_DEPRECATED IAlgorithmContext : public INoCopy                                                                                                                      
      |                      ^~~~~~~~~~~~~~~~~                                                                                                                                       
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10347:42: warning: 'IAlgorithm' is deprecated [-Wdeprecated-declarations]                                              
10347 |         int32_t nbAlgorithms) noexcept = 0;                                                                                                                                  
      |                                          ^                                                                                                                                   
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10261:22: note: declared here                                                                                          
10261 | class TRT_DEPRECATED IAlgorithm : public INoCopy                                                                                                                             
      |                      ^~~~~~~~~~                                                                                                                                              
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10365:35: warning: 'IAlgorithmSelector' is deprecated [-Wdeprecated-declarations]                                      
10365 | using IAlgorithmSelector = v_1_0::IAlgorithmSelector;                                                                                                                        
      |                                   ^~~~~~~~~~~~~~~~~~                                                                                                                         
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10309:22: note: declared here                                                                                          
10309 | class TRT_DEPRECATED IAlgorithmSelector : public IVersionedInterface                                                                                                         
      |                      ^~~~~~~~~~~~~~~~~~                                                                                                                                      
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10365:53: warning: 'IAlgorithmSelector' is deprecated [-Wdeprecated-declarations]                                      
10365 | using IAlgorithmSelector = v_1_0::IAlgorithmSelector;                                                                                                                        
      |                                                     ^                                                                                                                        
E:/projects/RVSFDT/libs/TensorRT-10.16.1.11/include/NvInfer.h:10309:22: note: declared here                                                                                          
10309 | class TRT_DEPRECATED IAlgorithmSelector : public IVersionedInterface                                                                                                         
      |                      ^~~~~~~~~~~~~~~~~~                                                                                                                                      
[ 96%] Linking CXX executable RVSFDT.exe
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(FilterChain.cpp.obj): in function `tbb::detail::d1::execution_slot(tbb::detail::d1::execution_data const&)':
C:/PROGRA~2/Intel/oneAPI/tbb/2022.3/include/tbb/../oneapi/tbb/detail/_task.h:214: undefined reference to `tbb::detail::r1::execution_slot(tbb::detail::d1::execution_data const*)'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(FilterChain.cpp.obj): in function `tbb::detail::d1::current_thread_index()':
C:/PROGRA~2/Intel/oneAPI/tbb/2022.3/include/tbb/../oneapi/tbb/task_arena.h:600: undefined reference to `tbb::detail::r1::execution_slot(tbb::detail::d1::execution_data const*)'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(YOLODetector.cpp.obj): in function `YOLODetector::YOLODetector(YOLOConfig)':
E:/projects/RVSFDT/src/core/Detection/YOLODetector.cpp:6: undefined reference to `LabelMap::loadCOCO80()'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(YOLODetector.cpp.obj): in function `YOLODetector::loadModel(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)':
E:/projects/RVSFDT/src/core/Detection/YOLODetector.cpp:16: undefined reference to `LabelMap::loadFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: E:/projects/RVSFDT/src/core/Detection/YOLODetector.cpp:18: undefined reference to `LabelMap::loadCOCO80()'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(YOLODetector.cpp.obj): in function `YOLODetector::postprocess(std::vector<cv::Mat, std::allocator<cv::Mat> > const&, cv::Size_<int> const&)':
E:/projects/RVSFDT/src/core/Detection/YOLODetector.cpp:158: undefined reference to `LabelMap::nameOf[abi:cxx11](int) const'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(DetectionRenderer.cpp.obj): in function `DetectionRenderer::render(cv::Mat&, std::vector<Detection, std::allocator<Detection> > const&) const':
E:/projects/RVSFDT/src/core/Detection/DetectionRenderer.cpp:14: undefined reference to `LabelMap::colorOf(int) const'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: CMakeFiles\RVSFDT.dir/objects.a(TensorRTBackend.cpp.obj): in function `TensorRTBackend::postprocess(float const*, int, cv::Size_<int> const&)':
E:/projects/RVSFDT/src/core/Detection/TensorRTBackend.cpp:267: undefined reference to `LabelMap::loadCOCO80()'
D:/Qt/Tools/mingw1310_64/bin/../lib/gcc/x86_64-w64-mingw32/13.1.0/../../../../x86_64-w64-mingw32/bin/ld.exe: E:/projects/RVSFDT/src/core/Detection/TensorRTBackend.cpp:274: undefined reference to `LabelMap::nameOf[abi:cxx11](int) const'
collect2.exe: error: ld returned 1 exit status
mingw32-make.exe[2]: *** [CMakeFiles\RVSFDT.dir\build.make:508: RVSFDT.exe] Error 1
mingw32-make.exe[1]: *** [CMakeFiles\Makefile2:87: CMakeFiles/RVSFDT.dir/all] Error 2
mingw32-make.exe: *** [Makefile:135: all] Error 2