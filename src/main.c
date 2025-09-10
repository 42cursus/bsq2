/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 04:15:13 by abelov            #+#    #+#             */
/*   Updated: 2025/09/09 04:25:10 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_sq bsqSolve(t_bsq bsq);
void delete_queue(t_tailhead *head);

/**
 * TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
 * click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
	FILE	*stream;
	t_bsq	bsq = {0x00};
	t_entry *n1, *np;
	/* Tail queue head */
	t_tailhead head = TAILQ_HEAD_INITIALIZER(head);

	if (argc != 2)
		exit((fprintf(stderr, "Usage: %s <file>\n", argv[0]), EXIT_FAILURE));
	{
		stream = fopen(argv[1], "r");
		if (stream == NULL)
			exit((fprintf(stderr, "fopen: %m"), EXIT_FAILURE));

		int		listSize = 0;
		char	*line = NULL;
		size_t	len = 0;
		ssize_t	nread;
		nread = getline(&line, &len, stream);
		if (nread != -1)
		{

			char	*endptr;
			*strchrnul(line, '\n') = '\0';
			bsq.legend.rows = (int)strtol(line, &endptr, 0);
			bsq.legend.empty = endptr[0];
			bsq.legend.obstacle = endptr[1];
			bsq.legend.full = endptr[2];
			bsq.cols = 0;

			while ((nread = getline(&line, &len, stream)) != -1) {
				*strchrnul(line, '\n') = '\0';
				if (bsq.cols == 0)
					bsq.cols = (int)__builtin_strlen(line);
				n1 = malloc(sizeof(struct qentry));
				n1->str = __builtin_strdup(line);
				TAILQ_INSERT_TAIL(&head, n1, entries);
				listSize++;
				if (listSize > bsq.cols)
					exit((delete_queue(&head), fprintf(stderr, "fopen: %m"), EXIT_FAILURE));
			}

			bsq.map = calloc(bsq.legend.rows, sizeof(char *));

			/* Forward traversal */
			int i = 0;
			TAILQ_FOREACH(np, &head, entries) bsq.map[i++] = np->str;
			t_sq result = bsqSolve(bsq);


			i = result.start.y;
			while (++i <= result.end.y)
			{
				char *row = bsq.map[i];
				int j = result.start.x;
				while (j < result.end.x)
					row[j++] = bsq.legend.full;
			}

			i = -1;
			while (++i < bsq.legend.rows)
			{
				fputs(bsq.map[i], stdout);
				fputs("\n", stdout);
			}
			free(bsq.map);
		}
		free(line);
		fclose(stream);
		delete_queue(&head);
	}
	exit(EXIT_SUCCESS);
}

t_sq bsqSolve(t_bsq bsq)
{
	if (!bsq.map) return  (t_sq){0x00};
	t_sq result = {0x00};

	struct s_legend legend = bsq.legend;

	int (*dp)[bsq.cols + 1] = calloc(2, sizeof(*dp));

	Point  p;
	int maxSideLength = 0;
	p.y = -1;
	while (++p.y < legend.rows)
	{
		size_t curr = p.y & 1;
		size_t prev = curr ^ 1;
		dp[0][0] = 0;
		p.x = 1;
		while (++p.x < bsq.cols + 1)
		{
			int up = dp[prev][p.x];
			int left = dp[curr][p.x - 1];
			int diag = dp[prev][p.x - 1];

			int minNeighbour = -1;
			if (bsq.map[p.y][p.x - 1] != legend.obstacle){
				minNeighbour = MIN(diag, MIN(up, left));
				if (minNeighbour + 1 > maxSideLength)
				{
					maxSideLength = minNeighbour + 1;
					result.end = p;
					result.start.x = p.x - maxSideLength;
					result.start.y =  p.y - maxSideLength;
				}
			}
			dp[curr][p.x] = minNeighbour + 1;
		}
	}
	free(dp);
	return result;
}

void delete_queue(t_tailhead *head)
{	/* TailQ deletion */
	struct qentry *n1, *n2;

	n1 = TAILQ_FIRST(head);
	while (n1 != NULL) {
		n2 = TAILQ_NEXT(n1, entries);
		free(n1->str);
		free(n1);
		n1 = n2;
	}
	TAILQ_INIT(head);
}
