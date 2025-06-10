/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_add_to_tokens.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbeauman <tbeauman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 19:02:18 by tbeauman          #+#    #+#             */
/*   Updated: 2025/06/10 18:35:17 by elopin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int     add_quotes_to_tokens(t_tokens **tokens, char *tmp, char *to_trim,
    t_token_type type)
{
	char	*tmp2;
    t_token_part *parts;
    t_tokens *new_tok;

    if (((type == SQUOTES && is_squotes(tmp)) && !ft_strchr(tmp, '$'))
    || ((type == DQUOTES && is_dquotes(tmp)) && !ft_strchr(tmp, '$')))
	{
        tmp2 = ft_strtrimv2(tmp, to_trim);
        if (!tmp2)
            return (0);
        new_tok = ft_new_token(tmp2, type, NULL);
        if (!new_tok)
        {
            free(tmp2);
            return (0);
        }
        ft_tokens_add_back(tokens, new_tok);
        return (1);
	}
    parts = split_token_by_quotes(tmp);
    if (!parts)
        return (0);
    new_tok = ft_new_token(NULL, MULTI, parts);
    if (!new_tok)
    {
        free(parts);
        return (0);
    }
    ft_tokens_add_back(tokens, new_tok);
    return (1);
}

t_token_part *create_token_part(char *content, t_quote_type quote, bool expand)
{
    t_token_part *part;
    
    part = malloc(sizeof(t_token_part));
    if (!part)
        return (NULL);
    part->content = content;
    part->quote = quote;
    part->expand = expand;
    part->next = NULL;
    return (part);
}

// Fonction helper pour ajouter une partie à la liste chaînée
void add_part_to_list(t_token_part **head, t_token_part *new_part)
{
    t_token_part *current;
    
    if (!*head)
    {
        *head = new_part;
        return;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_part;
}

int add_no_quotes_to_tokens(t_tokens **tokens, char *tmp)
{
    t_token_part *parts_head = NULL;
    t_token_part *new_part;
    char *current_part;
    int i;
    int start;
    int quote_start, quote_end;
    char quote_char;
    t_quote_type quote_type;
    bool has_quotes = false;
    t_tokens    *new_tok;

    i = 0;
    start = 0;
    while (tmp[i])
    {
        // Chercher la première quote
        if (tmp[i] == '\"' || tmp[i] == '\'')
        {
            has_quotes = true;
            quote_char = tmp[i];
            quote_start = i;
            
            // Ajouter la partie avant les quotes si elle existe
            if (i > start)
            {
                current_part = ft_substr(tmp, start, i - start);
                if (!current_part)
                    return (0);
                new_part = create_token_part(current_part, NONE, true);
                if (!new_part)
                {
                    free(current_part);
                    return (0);
                }
                add_part_to_list(&parts_head, new_part);
            }
            
            // Chercher la quote fermante
            i++; // passer la quote ouvrante
            quote_end = i;
            while (tmp[quote_end] && tmp[quote_end] != quote_char)
                quote_end++;
                
            if (tmp[quote_end] == quote_char) // Quote fermante trouvée
            {
                // Extraire le contenu entre quotes (sans les quotes)
                current_part = ft_substr(tmp, quote_start + 1, quote_end - quote_start - 1);
                if (!current_part)
                    return (0);
                    
                quote_type = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
                // Les single quotes n'expansent pas, les double quotes expansent
                new_part = create_token_part(current_part, quote_type, (quote_type == DOUBLE_QUOTE));
                if (!new_part)
                {
                    free(current_part);
                    return (0);
                }
                add_part_to_list(&parts_head, new_part);
                
                i = quote_end + 1; // continuer après la quote fermante
                start = i;
            }
            else // Quote non fermée, traiter comme texte normal
            {
                i = quote_start + 1;
            }
        }
        else
        {
            i++;
        }
    }
    
    if (start < i)
    {
        current_part = ft_substr(tmp, start, i - start);
        if (!current_part)
            return (0);
        new_part = create_token_part(current_part, NONE, true);
        if (!new_part)
        {
            free(current_part);
            return (0);
        }
        add_part_to_list(&parts_head, new_part);
    }
    
    // Si pas de quotes trouvées, créer un token simple NOQUOTES
    if (!has_quotes)
    {
        current_part = ft_strdup(tmp);
        if (!current_part)
            return (0);
        new_tok = ft_new_token(current_part, NOQUOTES, NULL);
        if (!new_tok)
        {
            free(current_part);
            return (0);
        }
        ft_tokens_add_back(tokens, new_tok);
        return (1);
    }
    
    // Créer le token MULTI avec toutes les parties
    
    new_tok = ft_new_token(NULL, MULTI, parts_head);
    if (!new_tok)
        return (0);
    
    ft_tokens_add_back(tokens, new_tok);
    return (1);
}

// static char *special_trim(char *tmp, int start, int end)
// {
//     char    *ret;
//     int     i;
//     int     j;

//     i = 0;
//     j = 0;
//     ret = (char*)malloc(sizeof(char) * (ft_strlen(tmp) - 1));
//     if (!ret)
//         return (NULL);
//     while (tmp[i] && j != start)
//     {
//         ret[j] = tmp[i];
//         j++;
//         i++;
//     }
//     i++;
//     while (tmp[i] && j != end)
//     {
//         ret[j] = tmp[i];
//         j++;
//         i++;
//     }
//     i++;
//     while (tmp[i])
//     {
//         ret[j] = tmp[i];
//         j++;
//         i++;
//     }
//     ret[j] = 0;
//     return (ret);
// }

// int     add_no_quotes_to_tokens(t_tokens **tokens, char *tmp)
// {
//     char    *tmp2;
//     int     i;
//     int    to_trim_start;
//     int    to_trim_end = 0;
//     t_token_type    type;

//     i = 0;
//     // fd_printf(2, "adding : %s\n", tmp);
//     while (tmp[i])
//     {
//         if (tmp[i] == '\"' || tmp[i] == '\'')
//         {
//             to_trim_start = i;
//             break ;
//         }
//         i++;
//     }
//     if (!tmp[i])
//     {
//         tmp2 = ft_strdup(tmp);
//         if (!tmp2)
//             return (0);
//         type = NOQUOTES;
//         ft_tokens_add_back(tokens, ft_new_token(tmp2, type, NULL));
//         return (1);
//     }
//     else
//     {
//         i = ft_strlen(tmp);
//         while (1)
//         {
//             if (tmp[i] == tmp[to_trim_start])
//             {
//                 to_trim_end = i - 1;
//                 break ;
//             }
//             i--;
//         }
//         if (tmp[to_trim_start] == '\'')
//             type = SQUOTES;
//         else
//             type = DQUOTES;
//         tmp2 = special_trim(tmp, to_trim_start, to_trim_end);
//         if (!tmp2)
//             return (0);
//         ft_tokens_add_back(tokens, ft_new_token(tmp2, type, NULL));
//         return (1);
//         }
// }


// int     add_quotes_to_tokens(t_tokens **tokens, char *tmp, char *to_trim,
//     t_token_type type)
// {
// 	char	*tmp2;
//     int     i;
//     t_tokens    *new_token;
//     t_lod       *lod;
//     t_lod       *new_lod;
//     int         sw;
//     bool        close;

//     i = 0;
//     lod = NULL;
//     sw = 0;
//     close = 0;
//     while (tmp[i])
//     {
//         if (tmp[i] == to_trim && !tmp[i + 1])
//         {
//             if (!sw)
//             {
//                 new_lod = ft_new_lod(0, close);
//                 if (!new_lod)
//                 {
//                     ft_clear_lod(&lod);
//                     return (0);
//                 }
//                 ft_lod_add_back(&lod, new_lod);
//                 close = !close;
//             }
//             new_lod = ft_new_lod(i, close);
//             if (!new_lod)
//             {
//                 ft_clear_lod(&lod);
//                 return (0);
//             }
//             ft_lod_add_back(&lod, new_lod);
//             close = !close;
//         }
//         i++;
//     }
//     if (lod)
//     {
//         new_lod = ft_new_lod(i - 1, close);
//         if (!new_lod)
//         {
//             ft_clear_lod(&lod);
//             return (0);
//         }
//         ft_lod_add_back(&lod, new_lod);
//     }
// 	tmp2 = ft_strtrimv2(tmp, to_trim);
// 	if (!tmp2)
// 		return (0);
// 	new_token = ft_new_token(tmp2, type);
//     if (!new_token)
//         return (0);
//     ft_tokens_add_back(tokens, ft_new_token(tmp2, type));
// 	return (1);

// }
