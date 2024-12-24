#ifndef ERROR_H
#define ERROR_H


typedef enum error_codes {
    FALLOC_SUCCESS           = 0x0,
    FALLOC_ERR_NO_MEM        = 0x11,
    FALLOC_ERR_INVALID_PTR   = 0x21,
    FALLOC_ERR_DOUBLE_FREE   = 0x31,    
    FALLOC_ERR_OUT_OF_BOUNDS = 0x41,  
    FALLOC_ERR_ALIGN_FAIL    = 0x51,    
    FALLOC_ERR_CORRUPTED     = 0x61,    
    FALLOC_ERR_NOT_ALLOCATED = 0x71   
} err_codes;



#endif // ERROR_H
