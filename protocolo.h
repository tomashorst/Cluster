#ifndef PROTO_H_
#define PROTO_H_ 1

#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include "Class_Job.h"

static const uint32_t INVALID_JOB_ID = -1;

typedef enum {
    CLIENT_SUBMIT_JOB,
    CLIENT_DELETE_JOB,
    CLIENT_QUERY_STATUS,
    JOB_STATUS,
    S2M_GET_JOB,
    M2S_RUN_JOB,
} Type;

typedef struct __attribute__((__packed__)) 
{
    uint8_t  type;
    uint16_t size8;
} Header;

inline static uint16_t getSize8(const Header *hdr)
{
    return ntohs(hdr->size8);
}

//Payload dependiendo el tipo de mensaje

struct  __attribute__((__packed__)) JobDef
{
   	char working_dir[PATH];         
   	char program[PATH];             
   	char args[ARGS];                
};

// CLIENT_SUBMIT_JOB
struct  __attribute__((__packed__)) SumbitJob
{
   	JobDef jobDef;
    uint8_t interactive;
    uint32_t interactive_address;           
    uint16_t interactive_nro_port;
};

// CLIENT_DELETE_JOB
struct __attribute__((__packed__)) DeleteJob
{
    uint32_t jobid;
};

// CLIENT_QUERY_STATUS
using QueryJob = DeleteJob;        //Como un typedef(le digo que tiene la misma estructura que DeleteJob)

// JOB_STATUS
struct  __attribute__((__packed__)) JobStatus
{
    uint32_t jobid;
    uint8_t status;
};

// M2S_RUN_JOB
struct __attribute__((__packed__)) RunJob
{
    uint32_t jobid;
   	JobDef jobDef;
    uint8_t interactive;
    uint32_t interactive_address;
    uint16_t interactive_nro_port;
};

// S2M_GET_JOB: no payload

typedef struct __attribute__((__packed__))
{
    Header hdr;

    union __attribute__((__packed__)) {
        SumbitJob submitJob; // CLIENT_SUBMIT_JOB
        DeleteJob deleteJob; // CLIENT_DELETE_JOB   
        QueryJob  queryJob;  // CLIENT_QUERY_STATUS
        JobStatus jobStatus; // JOB_STATUS
                             // S2M_GET_JOB: no payload
        RunJob    runJob;    // M2S_RUN_JOB
    } payload;

    uint8_t getType() const {
        return hdr.type;
    }
} Msg;

inline static void setSubmitJob(
    Msg *msg, 
    const char *working_dir,
    const char *program,
    const char *args,
    uint8_t interactive,
    uint32_t interactive_address,           
    uint16_t interactive_nro_port)
{
    msg->hdr.type = CLIENT_SUBMIT_JOB;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(SumbitJob));
    strncpy(msg->payload.submitJob.jobDef.working_dir, working_dir, PATH);
    strncpy(msg->payload.submitJob.jobDef.program, program, PATH);
    strncpy(msg->payload.submitJob.jobDef.args, args, ARGS);
    msg->payload.submitJob.interactive = interactive;
    msg->payload.submitJob.interactive_address = interactive_address;
    msg->payload.submitJob.interactive_nro_port = interactive_nro_port;
}

inline static void getSubmitJob(
    const Msg *msg,
    char working_dir[PATH],
   	char program[PATH],
   	char args[ARGS],
    uint8_t *interactive,
    uint32_t *interactive_address,
    uint16_t *nro_port_interactive)
{
    if(msg->hdr.type != CLIENT_SUBMIT_JOB
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(SumbitJob) )
    {
        throw runtime_error("Invalid CLIENT_SUBMIT_JOB");
    }
    *nro_port_interactive = msg->payload.submitJob.interactive_nro_port;
    *interactive_address = msg->payload.submitJob.interactive_address;
    *interactive = msg->payload.submitJob.interactive;
    strncpy(args, msg->payload.submitJob.jobDef.args, ARGS);
    strncpy(program, msg->payload.submitJob.jobDef.program, PATH);
    strncpy(working_dir, msg->payload.submitJob.jobDef.working_dir, PATH);
}

inline static void setDeleteJob(
    Msg *msg,
    uint32_t jobid)
{
    msg->hdr.type = CLIENT_DELETE_JOB;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(DeleteJob));
    msg->payload.deleteJob.jobid = htonl(jobid);
}

inline static void getDeleteJob(
    const Msg *msg,
    uint32_t *jobid)    
{
    if(msg->hdr.type != CLIENT_DELETE_JOB
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(DeleteJob) )
    {
        throw runtime_error("Invalid CLIENT_DELETE_JOB");
    }
    *jobid = ntohl(msg->payload.deleteJob.jobid);
}

inline static void setQueryJob(
    Msg *msg,
    uint32_t jobid)
{
    msg->hdr.type = CLIENT_QUERY_STATUS;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(QueryJob));
    msg->payload.queryJob.jobid = htonl(jobid);
}

inline static void getQueryJob(
    const Msg *msg,
    uint32_t *jobid)    
{
    if(msg->hdr.type != CLIENT_QUERY_STATUS
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(QueryJob) )
    {
        throw runtime_error("Invalid CLIENT_QUERY_STATUS");
    }
    *jobid = ntohl(msg->payload.queryJob.jobid);
}

inline static void setJobStatus(
    Msg *msg,
    uint32_t jobid,
    uint8_t status)
{
    msg->hdr.type = JOB_STATUS;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(JobStatus));
    msg->payload.jobStatus.jobid = htonl(jobid);   
    msg->payload.jobStatus.status = status;
}

inline static void setJobStatus(
    Msg *msg,
    uint32_t jobid,
    ESTADO status)
{
    setJobStatus(msg, jobid, (uint8_t) status);
}

inline static void getJobStatus(
    const Msg *msg,
    uint32_t *jobid,        
    uint8_t *status)
{
    if(msg->hdr.type != JOB_STATUS
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(JobStatus) )
    {
        throw runtime_error("Invalid JOB_STATUS");
    }
    *jobid = ntohl(msg->payload.jobStatus.jobid);
    *status = msg->payload.jobStatus.status;
}

inline static void setGetJob(
    Msg *msg)
{
    msg->hdr.type = S2M_GET_JOB;
    msg->hdr.size8 = htons(sizeof(Header));
}

inline static void getGetJob(
    const Msg *msg)
{
    if(msg->hdr.type != S2M_GET_JOB
        || ntohs(msg->hdr.size8) != sizeof(Header))
    {
        throw runtime_error("Invalid S2M_GET_JOB");
    }
}

inline static void setRunJob(
    Msg *msg,
    uint32_t jobid,
    const char *working_dir,
    const char *program,
    const char *args)
{
    msg->hdr.type = M2S_RUN_JOB;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(RunJob));
    msg->payload.runJob.jobid = htonl(jobid);
    strncpy(msg->payload.runJob.jobDef.working_dir, working_dir, PATH);
    strncpy(msg->payload.runJob.jobDef.program, program, PATH);
    strncpy(msg->payload.runJob.jobDef.args, args, ARGS);   
}

//Para el interactivo

inline static void setRunJob(
    Msg *msg,
    uint32_t jobid,
    const char *working_dir,
    const char *program,
    const char *args,
    uint8_t interactive,
    uint32_t interactive_address,
    uint16_t interactive_nro_port)
{
    msg->hdr.type = M2S_RUN_JOB;
    msg->hdr.size8 = htons(sizeof(Header) + sizeof(RunJob));
    msg->payload.runJob.jobid = htonl(jobid);
    strncpy(msg->payload.runJob.jobDef.working_dir, working_dir, PATH);
    strncpy(msg->payload.runJob.jobDef.program, program, PATH);
    strncpy(msg->payload.runJob.jobDef.args, args, ARGS);   
    msg->payload.runJob.interactive = interactive;
    msg->payload.runJob.interactive_address = interactive_address;
    msg->payload.runJob.interactive_nro_port = htons(interactive_nro_port);
}

inline static void getRunJob(
    const Msg *msg,
    uint32_t *jobid,
    char working_dir[PATH],
    char program[PATH],
    char args[ARGS]
    )
{
    if(msg->hdr.type != M2S_RUN_JOB
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(RunJob) )
    {
        throw runtime_error("Invalid M2S_RUN_JOB");
    }
    *jobid = ntohl(msg->payload.runJob.jobid);
    strncpy(args, msg->payload.runJob.jobDef.args, ARGS);
    strncpy(program, msg->payload.runJob.jobDef.program, PATH);
    strncpy(working_dir, msg->payload.runJob.jobDef.working_dir, PATH);
    

}

//Para el interactivo

inline static void getRunJob(
    const Msg *msg,
    uint32_t *jobid,
    char working_dir[PATH],
    char program[PATH],
    char args[ARGS],
    uint8_t *interactive,
    uint32_t *interactive_address,
    uint16_t *interactive_nro_port)
{
    if(msg->hdr.type != M2S_RUN_JOB
        || ntohs(msg->hdr.size8) != sizeof(Header) + sizeof(RunJob) )
    {
        throw runtime_error("Invalid M2S_RUN_JOB");
    }
    *jobid = ntohl(msg->payload.runJob.jobid);
    strncpy(args, msg->payload.runJob.jobDef.args, ARGS);
    strncpy(program, msg->payload.runJob.jobDef.program, PATH);
    strncpy(working_dir, msg->payload.runJob.jobDef.working_dir, PATH);
    *interactive = msg->payload.runJob.interactive;
    *interactive_address = msg->payload.runJob.interactive_address;
    *interactive_nro_port = ntohs(msg->payload.runJob.interactive_nro_port);
}

void printType(const Msg *m);

int sendMsg(int sockfd, const Msg *msg);
int recvMsg(int sockfd, Msg *msg);

#endif // PROTO_H_
