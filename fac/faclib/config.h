#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "array.h"
#include "global.h"
#include "dbase.h"
#include "nucleus.h"

#define MAX_SPEC_SYMBOLS   14
#define LEVEL_NAME_LEN     128
#define GROUP_NAME_LEN     20
#define MAX_GROUPS         512
#define MAX_SYMMETRIES     0x200
#define MAX_STATES_PER_SYM 0x200000
#define CONFIGS_BLOCK      256
#define STATES_BLOCK       512

/* a SHELL is a set of qunatum #s, the principal quantum number n, 
   the relativistic angular quantum number kappa. */
typedef struct _SHELL_ {
  int n;
  int kappa;
  int nq;
} SHELL;
  
/* a SHELL_STATE specify the seneority and the total angular momentum of 
   a shell with any occupation, along with the total angular momentum when
   this shell is coupled to its next inner shell. if this is the inner most 
   shell, this angular momentum is the same as the shell total_j. 
   All angular momenta are represented by the double of its actual value.*/  
typedef struct _SHELL_STATE_{
  int shellJ;
  int totalJ;
  int nu; 
  int Nr; /* Nr is the additional quantum # may need to specify the state */
} SHELL_STATE;
#define GetShellJ(s) ((s).shellJ)
#define GetTotalJ(s) ((s).totalJ)
#define GetNu(s)     ((s).nu)
#define GetNr(s)     ((s).Nr)

/* in config, shells are ordered in reverse order, i.e., outer shells come
   first.  */
typedef struct _CONFIG_ {
  int n_shells;
  int n_csfs;
  SHELL *shells;
  SHELL_STATE *csfs; /* all coupled states */
} CONFIG;

typedef struct _AVERAGE_CONFIG_ {
  int n_cfgs;
  int n_shells;
  int *n;
  int *kappa;
  double *nq;
} AVERAGE_CONFIG;

typedef struct _CONFIG_GROUP_ {
  char name[GROUP_NAME_LEN];  
  int n_cfgs;
  ARRAY cfg_list;
} CONFIG_GROUP;

typedef struct _STATE_ {
  int kgroup;
  int kcfg;
  int kstate;
} STATE;

typedef struct _SYMMETRY_ {
  int n_states;
  ARRAY states;
} SYMMETRY;

int Couple(CONFIG *cfg);
int CoupleOutmost(CONFIG *cfg, CONFIG *outmost, CONFIG *inner);
int GetSingleShell(CONFIG *cfg);

/******************************************************************** 
   utilities for packing and unpacking a shell. 
   note that all angular momenta are represented by an integer, which is
   the double of its actual value, including the orbital angular momentum.
*********************************************************************/
void UnpackShell(SHELL *s, int *n, int *kl, int *j, int *nq);
void PackShell(SHELL *s, int n, int kl, int j, int nq); 
int GetJ(SHELL *shell);
int GetL(SHELL *shell);
int GetNq(SHELL *shell);
void GetJLFromKappa(int kappa, int *j, int *kl);
int GetLFromKappa(int kappa); 
int GetJFromKappa(int kappa);
int GetKappaFromJL(int j, int kl); 
int CompareShell(SHELL *s1, SHELL *s2);
int ShellClosed(SHELL *s);

/* utilities for packing and unpacking a shell state */
void PackShellState(SHELL_STATE *s, int J, int j, int nu, int Nr);

/* determine the average configuration for a given set of groups */
int GetAverageConfig(int ng, int *kg, double *weight,
		     int n_screen, int *screened_n, double screened_charge,
		     AVERAGE_CONFIG *acfg);

/* routines handle configuration groups */
int GroupIndex(char *name);
int GroupExists(char *name);
int AddConfigToList(int k, CONFIG *cfg);
int AddGroup(char *name);
CONFIG_GROUP *GetGroup(int k);
CONFIG_GROUP *GetNewGroup();
int GetNumGroups();
CONFIG *GetConfig(STATE *s);

/* routines handle the symmetry list */
int AddStateToSymmetry(int kg, int kc, int kstate, int parity, int j);
int AddConfigToSymmetry(int kg, int kc, CONFIG *cfg);
SYMMETRY *GetSymmetry(int k);
void DecodePJ(int i, int *p, int *j);

int SpecSymbol(char *s, int kl);

int InitConfig();

#endif
