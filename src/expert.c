#include "php.h"
#include "inquiry.h"
#include "expert.h"

hypx_therapeutic_inquiry hypx_expert_make_inquiry(int phase) {
    hypx_therapeutic_inquiry inquiry;
    void * inquiry_content;

    inquiry = emalloc(sizeof(struct _hypx_therapeutic_inquiry_struct));
    if (inquiry == NULL) return NULL;
    inquiry->inq_alt = NULL;
    inquiry->inq_type = HYPX_INQUIRY_TYPE_RADIOS;
    struct _hypx_comrads_inquiry_content * content = 
        emalloc(sizeof(struct _hypx_comrads_inquiry_content));
    if (content == NULL) { efree(inquiry); return NULL; }
    inquiry->inq_numalt = 2;
    content->crinq_numbuttons = 2; 
    content->crinq_question = "V katero smer se je vrtela plesalka?";
    content->crinq_label[0] = "levo";
    content->crinq_label[1] = "desno";
    inquiry_content = content;
    inquiry->inq_content = inquiry_content;
    return inquiry;
}
