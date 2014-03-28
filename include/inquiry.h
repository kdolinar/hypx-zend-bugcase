#ifndef _HYPX_INQUIRY_H_
#define _HYPX_INQUIRY_H_

#define HYPX_MAXNUMALT 128

#define HYPX_INQUIRY_TYPE_RADIOS      2
struct _hypx_therapeutic_inquiry_struct {
    unsigned char inq_type;
    unsigned long long inq_id;
    void * inq_content;
    int inq_numalt;
    void * inq_alt;
    int inq_links;
};

typedef struct _hypx_therapeutic_inquiry_struct * hypx_therapeutic_inquiry;

struct _hypx_comrads_inquiry_content {
    char * crinq_question;
    int crinq_numbuttons;
    char * crinq_label[HYPX_MAXNUMALT];
};
 
#endif
