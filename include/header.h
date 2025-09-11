/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 04:32:12 by abelov            #+#    #+#             */
/*   Updated: 2025/09/09 04:32:12 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
#define HEADER_H

#define _GNU_SOURCE
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/param.h>

typedef struct qentry {
	char *str;
	TAILQ_ENTRY(qentry) entries;             /* Tail queue */
} t_entry;

typedef TAILQ_HEAD(tailhead, qentry) t_tailhead;

typedef struct Point
{
	int	x,y;
}	Point;

typedef struct s_sq
{
	Point	start,end;
}	t_sq;

typedef struct Dim
{
	int	rows, cols;
}	Dim;

typedef struct s_bsq
{
	char	**map;
	int		cols;
	struct s_legend
	{
		char	obstacle;
		char	empty;
		char	full;
		int		rows;
	}	legend;
}	t_bsq;

#endif //HEADER_H
