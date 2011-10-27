// COPYRIGHT NOTICE TBD NOT FOR RELEASE

#include "NE10.h"

#include <stdio.h>

#define CPUINFO_BUFFER_SIZE  (1024*4)

// This local variable indicates whether or not the running platform supports ARM NEON
arm_result_t is_NEON_available = NE10_ERR;

arm_result_t NE10_HasNEON()
{
    return is_NEON_available;
}

arm_result_t NE10_init()
{
    FILE*   infofile = NULL;               // To open the file /proc/cpuinfo
    char    cpuinfo[CPUINFO_BUFFER_SIZE];  // The buffer to read in the string
    size_t  bytes = 0;                     // Numbers of bytes read from the file
    int     i = 0;                         // Temporary loop counter

    memset( cpuinfo, 0, CPUINFO_BUFFER_SIZE );
    infofile = fopen( "/proc/cpuinfo", "r" );
    bytes    = fread( cpuinfo, 1, sizeof(cpuinfo), infofile );
    fclose( infofile );

    if( 0 == bytes || CPUINFO_BUFFER_SIZE == bytes )
    {
        fprintf( stderr, "ERROR: Couldn't read the file \"/proc/cpuinfo\". NE10_init() failed.\n");
        return NE10_ERR;
    }

    while( '\0' != cpuinfo[i] ) cpuinfo[i++] = (char)tolower(cpuinfo[i]);

    if ( 0 != strstr(cpuinfo, "neon") )
    {
       is_NEON_available = NE10_OK;
    }

    if ( NE10_OK == NE10_HasNEON() )
    {
      addc_float = addc_float_neon;
      addc_vec2f = addc_vec2f_neon;
      addc_vec3f = addc_vec3f_neon;
      addc_vec4f = addc_vec4f_neon;
      subc_float = subc_float_neon;
      subc_vec2f = subc_vec2f_neon;
      subc_vec3f = subc_vec3f_neon;
      subc_vec4f = subc_vec4f_neon;
      rsbc_float = rsbc_float_neon;
      rsbc_vec2f = rsbc_vec2f_neon;
      rsbc_vec3f = rsbc_vec3f_neon;
      rsbc_vec4f = rsbc_vec4f_neon;
      mulc_float = mulc_float_neon;
      mulc_vec2f = mulc_vec2f_neon;
      mulc_vec3f = mulc_vec3f_neon;
      mulc_vec4f = mulc_vec4f_neon;
      divc_float = divc_float_neon;
      divc_vec2f = divc_vec2f_neon;
      divc_vec3f = divc_vec3f_neon;
      divc_vec4f = divc_vec4f_neon;
      setc_float = setc_float_neon;
      setc_vec2f = setc_vec2f_neon;
      setc_vec3f = setc_vec3f_neon;
      setc_vec4f = setc_vec4f_neon;
      mlac_float = mlac_float_neon;
      mlac_vec2f = mlac_vec2f_neon;
      mlac_vec3f = mlac_vec3f_neon;
      mlac_vec4f = mlac_vec4f_neon;
      add_float = add_float_neon;
      sub_float = sub_float_neon;
      mul_float = mul_float_neon;
      div_float = div_float_neon;
      mla_float = mla_float_neon;
      abs_float = abs_float_neon;
      len_vec2f = len_vec2f_neon;
      len_vec3f = len_vec3f_neon;
      len_vec4f = len_vec4f_neon;
      normalize_vec2f = normalize_vec2f_neon;
      normalize_vec3f = normalize_vec3f_neon;
      normalize_vec4f = normalize_vec4f_neon;
    }
    else
    {
      addc_float = addc_float_c;
      addc_vec2f = addc_vec2f_c;
      addc_vec3f = addc_vec3f_c;
      addc_vec4f = addc_vec4f_c;
      subc_float = subc_float_c;
      subc_vec2f = subc_vec2f_c;
      subc_vec3f = subc_vec3f_c;
      subc_vec4f = subc_vec4f_c;
      rsbc_float = rsbc_float_c;
      rsbc_vec2f = rsbc_vec2f_c;
      rsbc_vec3f = rsbc_vec3f_c;
      rsbc_vec4f = rsbc_vec4f_c;
      mulc_float = mulc_float_c;
      mulc_vec2f = mulc_vec2f_c;
      mulc_vec3f = mulc_vec3f_c;
      mulc_vec4f = mulc_vec4f_c;
      divc_float = divc_float_c;
      divc_vec2f = divc_vec2f_c;
      divc_vec3f = divc_vec3f_c;
      divc_vec4f = divc_vec4f_c;
      setc_float = setc_float_c;
      setc_vec2f = setc_vec2f_c;
      setc_vec3f = setc_vec3f_c;
      setc_vec4f = setc_vec4f_c;
      mlac_float = mlac_float_c;
      mlac_vec2f = mlac_vec2f_c;
      mlac_vec3f = mlac_vec3f_c;
      mlac_vec4f = mlac_vec4f_c;
      add_float = add_float_c;
      sub_float = sub_float_c;
      mul_float = mul_float_c;
      div_float = div_float_c;
      mla_float = mla_float_c;
      abs_float = abs_float_c;
      len_vec2f = len_vec2f_c;
      len_vec3f = len_vec3f_c;
      len_vec4f = len_vec4f_c;
      normalize_vec2f = normalize_vec2f_c;
      normalize_vec3f = normalize_vec3f_c;
      normalize_vec4f = normalize_vec4f_c;
    }
}

// These are actual definitions of our function pointers that are declared in inc/NE10.h
arm_result_t (*addc_float)(arm_float_t * dst, arm_float_t * src, const arm_float_t cst, unsigned int count);
arm_result_t (*addc_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*addc_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*addc_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*subc_float)(arm_float_t * dst, arm_float_t * src, const arm_float_t cst, unsigned int count);
arm_result_t (*subc_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*subc_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*subc_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*rsbc_float)(arm_float_t * dst, arm_float_t *src, const arm_float_t cst, unsigned int count);
arm_result_t (*rsbc_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*rsbc_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*rsbc_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*mulc_float)(arm_float_t * dst, arm_float_t * src, const arm_float_t cst, unsigned int count);
arm_result_t (*mulc_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*mulc_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*mulc_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*divc_float)(arm_float_t * dst, arm_float_t * src, const arm_float_t cst, unsigned int count);
arm_result_t (*divc_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*divc_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*divc_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*setc_float)(arm_float_t * dst, const arm_float_t cst, unsigned int count);
arm_result_t (*setc_vec2f)(arm_vec2f_t * dst, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*setc_vec3f)(arm_vec3f_t * dst, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*setc_vec4f)(arm_vec4f_t * dst, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*mlac_float)(arm_float_t * dst, arm_float_t * acc, arm_float_t * src, const arm_float_t cst, unsigned int count);
arm_result_t (*mlac_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * acc, arm_vec2f_t * src, const arm_vec2f_t * cst, unsigned int count);
arm_result_t (*mlac_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * acc, arm_vec3f_t * src, const arm_vec3f_t * cst, unsigned int count);
arm_result_t (*mlac_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * acc, arm_vec4f_t * src, const arm_vec4f_t * cst, unsigned int count);
arm_result_t (*add_float)(arm_float_t * dst, arm_float_t * src1, arm_float_t * src2, unsigned int count);
arm_result_t (*sub_float)(arm_float_t * dst, arm_float_t * src1, arm_float_t * src2, unsigned int count);
arm_result_t (*mul_float)(arm_float_t * dst, arm_float_t * src1, arm_float_t * src2, unsigned int count);
arm_result_t (*div_float)(arm_float_t * dst, arm_float_t * src1, arm_float_t * src2, unsigned int count);
arm_result_t (*mla_float)(arm_float_t * dst, arm_float_t * acc, arm_float_t * src1, arm_float_t * src2, unsigned int count);
arm_result_t (*abs_float)(arm_float_t * dst, arm_float_t * src, unsigned int count);
arm_result_t (*len_vec2f)(arm_float_t * dst, arm_vec2f_t * src, unsigned int count);
arm_result_t (*len_vec3f)(arm_float_t * dst, arm_vec3f_t * src, unsigned int count);
arm_result_t (*len_vec4f)(arm_float_t * dst, arm_vec4f_t * src, unsigned int count);
arm_result_t (*normalize_vec2f)(arm_vec2f_t * dst, arm_vec2f_t * src, unsigned int count);
arm_result_t (*normalize_vec3f)(arm_vec3f_t * dst, arm_vec3f_t * src, unsigned int count);
arm_result_t (*normalize_vec4f)(arm_vec4f_t * dst, arm_vec4f_t * src, unsigned int count);

