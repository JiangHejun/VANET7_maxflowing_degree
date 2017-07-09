#ifndef __STABLE_H__
#define __STABLE_H__ __ZIE_IUK__

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "real_map.h"
#include <queue>

#define MAXN NUM_REQ+NUM_U*T+NUM_V*T+2
#define INF INT_MAX
#define MIN(a,b) ((a)<(b)?(a):(b))
#define percent 0.5
#define RANK 3

int max_flow(struct vehicle *v);

#endif