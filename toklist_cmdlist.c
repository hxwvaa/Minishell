#include "minishell.h"


t_cmd *our_clstlast(t_cmd *lst)
{
    t_cmd *ptr;

    if(!lst)
        return (NULL);
    ptr = lst;
    while(ptr->next != NULL)
        ptr = ptr->next;
    return(ptr);
}
void our_clstadd_back(t_cmd **lst, t_cmd *new)
{
    if(!new)
        return ;
    if(!*lst)
        *lst = new;
    else
        our_clstlast(*lst)->next = new;
}
// t_cmd *our_clistnew(char *cmd, int count)
// {
//     t_cmd   *list;

//     //list = malloc(sizeof(t_cmd));
//     list = ft_calloc(1, sizeof(t_cmd));
//     if(!list)
//         return (NULL);
//     list->cmd = ft_strdup(cmd);
//     if(!list->cmd)
//         return (free(list), (NULL));
//     //list->cargs = malloc((sizeof (char *)) * (count + 1));
//     list->cargs = ft_calloc((count + 1), sizeof(char *));
//     if(!list->cargs)
//         return (free(list->cmd), free(list), (NULL));
//     list->cargs[0] = ft_strdup(cmd);
//     if(!list->cargs[0])
//         return (free(list->cargs), free(list->cmd), free(list), NULL);
//     list->cargs[1] = NULL;
//     list->app = 0;
//     list->inf = NULL;
//     list->outf = NULL;
//     list->limiter = NULL;
//     list->hd_fd = -1;
//     list->next = NULL;
//     return(list);
// }

t_cmd *our_clistnew(int count)
{
    t_cmd   *list;

    //list = malloc(sizeof(t_cmd));
    list = ft_calloc(1, sizeof(t_cmd));
    if(!list)
        return (NULL);
    list->cmd = NULL;
    // if(!list->cmd)
    //     return (free(list), (NULL));
    //list->cargs = malloc((sizeof (char *)) * (count + 1));
    list->cargs = ft_calloc((count + 1), sizeof(char *));
    if(!list->cargs)
        return (free(list), (NULL));
    list->cargs[0] = NULL;
    // if(!list->cargs[0])
    //     return (free(list->cargs), free(list->cmd), free(list), NULL);
    //list->cargs[1] = NULL;
    list->app = 0;
    list->inf = NULL;
    list->outf = NULL;
    list->limiter = NULL;
    list->hd_input = NULL;
    //list->hd_fd = -1;
    list->next = NULL;
    return(list);
}

int count_args(t_toklist *list)
{
    int i;
    t_toklist *temp;

    i = 0;
    temp = list;
    while(temp && temp->type != PIPE)
    {
        if(temp->type == CMD || temp->type == FLAG || temp->type == ARGS)
            i++;
        temp = temp->next;
    }
    return (i);
}

// int count_cargs(t_cmd *cmd)
// {
//     int i;
//     t_cmd*temp;

//     i = 0;
//     temp = list;
//     while(temp->cargs[i])
//     {  
//             i++;
//     }
//     return (i);
// }


t_cmd *our_toklist_cmdlist(t_toklist *list, t_shell *data)
{
    int i;
    t_cmd *new;
    t_cmd *curr;
    t_toklist *temp;

    new = NULL;
    curr = NULL;
    i = 0;
    int j = 1;
    temp = list;
    while(temp)
    {
        if (j == 1)
        {
            new = our_clistnew(count_args(temp));
            if(!new)
                return (perror("malloc"), NULL);
            if(new)
                our_clstadd_back(&data->cmds, new);
            curr = new;
            j = 0;
        }
        if(temp->type == CMD)
        {
            //new = our_clistnew(temp->token, count_args(list));
            //if(!new)
                //return (perror("malloc"), NULL);
            // if(new)
            //     our_clstadd_back(&data->cmds, new);
            
            //curr = new;
            curr->cmd = ft_strdup(temp->token); // protect mallocs
            curr->cargs[0] = ft_strdup(temp->token);
            i = 1;
            int j = count_args(temp);
            temp = temp->next;
            while(temp && temp->type != PIPE)
            {
                if(temp->type == CMD || temp->type == FLAG || temp->type == ARGS)
                {
                    //memory leak if multiple redirection in command line ithink
                    //eg.: asd > asd | asad >asdfdsd >agf 
                    //idea for solution add enum for infile outfile & limiter,
                    //and set token type when encountered
                    if(i <= j)
                        curr->cargs[i++] = ft_strdup(temp->token);
                //printf("dsf\n");
                    //temp = temp->next;
                }
                if(temp->type == REDIR_IN || temp->type == HERE_DOC)
                {
                    if(temp && temp->type == HERE_DOC)
                    {
                        temp = temp->next;
                        curr->limiter = ft_strdup(temp->token);
                    }
                    else
                    {
                        temp = temp->next;
                        curr->inf = ft_strdup(temp->token);
                    }
                }
                if(temp->type == REDIR_OUT || temp->type == APPEND)
                {
                    if(temp && temp->type == APPEND)
                    {
                        temp = temp->next;
                        curr->outf = ft_strdup(temp->token);
                        curr->app = 1;
                    }
                    else
                    {
                        temp = temp->next;
                        curr->outf = ft_strdup(temp->token);
                        curr->app = 0;
                    }
                }
                temp = temp->next;
            }
            curr->cargs[i] = NULL;
            
        }
        else if(temp->type == REDIR_IN || temp->type == HERE_DOC)
        {
            if(temp && temp->type == HERE_DOC)
            {
                temp = temp->next;
                curr->limiter = ft_strdup(temp->token);
            }
            else
            {
                temp = temp->next;
                curr->inf = ft_strdup(temp->token);
            }
        }
        else if(temp->type == REDIR_OUT || temp->type == APPEND)
        {
            if(temp && temp->type == APPEND)
            {
                temp = temp->next;
                curr->outf = ft_strdup(temp->token);
                curr->app = 1;
            }
            else
            {
                temp = temp->next;
                curr->outf = ft_strdup(temp->token);
                curr->app = 0;
            }
        }
        else if(temp->type == PIPE)
        {
            curr = NULL;
            j = 1;
            temp = temp->next;
        }
        else
            temp = temp->next;
    }
    return(data->cmds);
}
