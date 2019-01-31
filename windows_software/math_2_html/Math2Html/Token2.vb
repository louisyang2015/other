Public Class Token2
    Public token_string As String
    Public token_type As TokenType2


    Public Sub New()
        token_string = New String("")
        token_type = TokenType2.simple_expression
    End Sub


    '*** this function takes in a string and generates an array of
    '*** Token2 objects.
    '*** Example:
    '*** input = 
    '***    normal text some normal, text _sum(_root(7, 3 + 4)_, x + y + z)_ , _sum3(x)_ + normal text
    '*** return value = 
    '***    return_val(0).token_string	"normal text some normal, text "
    '***    return_val(1).token_string	"_sum(_root(7, 3 + 4)_, x + y + z)_"
    '***    return_val(2).token_string	" , _sum3(x)_ + normal text"
    '***    return_val(0).token_type	simple_expression
    '***    return_val(1).token_type	a_function
    '***    return_val(2).token_type	simple_expression
    Public Shared Function tokenize(ByVal math_expression As String) As Token2()
        Dim return_val(10) As Token2

        ' Rightnow all 11 return_val elements are empty
        ' We will fill in return_val(0) first, then
        ' return_val(1) and so forth. The index "j" denotes
        ' which return_val element we are on. At the end, we
        ' will resize return_val so there are no "empty" 
        ' return_val elements
        Dim j As Integer = 0

        ' allocate memory for item zero
        return_val(0) = New Token2()

        ' go over the entire string character by character
        Dim i As Integer = 0
        For i = 0 To math_expression.Length - 1
            ' whenever there is an an underscore, it might be
            ' the beginning of a function that render_complex
            ' handles
            If math_expression.Chars(i) = "_" Then
                'we need to know if this is really the start
                'of a function that render_complex handles
                Dim is_function As Boolean = False

                ' end_index = location of the ending underscore that marks the function
                Dim end_index As Integer = find_next_underscore(math_expression, i + 1)

                ' first_paren = location of the first '(' after the current underscore
                Dim first_paren As Integer = math_expression.IndexOf("(", i)

                ' a function looks like: _name(...)_, so to be a function at all:
                If end_index > i _
                AndAlso first_paren > i + 1 _
                AndAlso end_index > first_paren _
                AndAlso math_expression.Chars(end_index - 1) = ")" Then
                    '** at this point, math_expression(i) to 
                    '** math_expression(end_index) at least is in
                    '** the format of being a function

                    Dim function_name As String
                    function_name = math_expression.Substring(i + 1, first_paren - i - 1)
                    Dim list_of_functions_obj As list_of_functions = New list_of_functions()
                    is_function = list_of_functions_obj.find(function_name)
                End If

                If is_function = False Then
                    'this is the basic case:
                    return_val(j).token_string = return_val(j).token_string + math_expression.Chars(i)
                Else
                    ' #1. We might have to create a new token to
                    ' hold the current function_macro text segment
                    ' #2. Then afterward we have to create a second
                    ' new token as preparation for the next loop

                    'start a new token if necessary (#1)
                    If return_val(j).token_string.Length > 0 Then
                        j = j + 1
                        ' enlarge return_val if necessary
                        If j > return_val.Length - 1 Then
                            ReDim Preserve return_val(j + 10)
                        End If
                        return_val(j) = New Token2()
                    End If

                    ' we do want both the starting and ending underscores in 
                    ' token_string
                    return_val(j).token_string = math_expression.Substring(i, end_index - i + 1)
                    return_val(j).token_type = TokenType2.a_function

                    ' for the next loop, we want i to be at end_index + 1
                    i = end_index

                    ' now for the #2 part --- preparation for the
                    ' next loop
                    If end_index < math_expression.Length - 1 Then
                        j = j + 1
                        ' enlarge return_val if necessary
                        If j > return_val.Length - 1 Then
                            ReDim Preserve return_val(j + 10)
                        End If
                        return_val(j) = New Token2()
                    End If

                End If
                '* end of the block handling math_expression.Chars(i) = "_"

            Else
                'this is the basic case
                return_val(j).token_string = return_val(j).token_string + math_expression.Chars(i)
            End If

        Next

        ReDim Preserve return_val(j)
        Return return_val
    End Function


    ' note the following function and comments are copied
    ' from Token.vb 
    '*** This function looks for the next underscore taking into
    '*** account the situation we face in this program --- 
    '*** ensuring matching parenthesis pairs
    '*** If no underscore is found, the function returns -1
    Private Shared Function find_next_underscore(ByVal math_expression As String, ByVal start_index As Integer) As Integer
        '*** given math_expression = 
        '   ..._root( , b_exp(2)_ - 4ac)_...
        '*** we want the underscore after -4ac) as the ending
        '*** location of the function. We do not want the first
        '*** underscore to mark as the end location of the 
        '*** function
        '
        ' The solution is to look for an undescore, but as
        ' soon as we see an '(' , we look for the next ')'
        ' Until we have enough ')' to match '(', the 
        ' underscores that we see doesn't count.

        Dim return_val As Integer = -1

        ' num_left_paren = the number of unmatched '(' 
        ' that we encountered so far
        Dim num_left_paren As Integer = 0

        Dim i As Integer = start_index
        For i = start_index To math_expression.Length - 1
            'the basic case
            If math_expression.Chars(i) = "_" _
            And num_left_paren = 0 Then
                return_val = i
                Exit For
            End If

            'count up if we see the left parenthesis
            If math_expression.Chars(i) = "(" Then
                num_left_paren = num_left_paren + 1
            End If

            'count down if we see the right parenthesis
            If math_expression.Chars(i) = ")" Then
                num_left_paren = num_left_paren - 1
            End If
        Next

        Return return_val

    End Function

End Class

Public Enum TokenType2
    simple_expression = 0
    a_function = 1
End Enum

'*** This class contains a list of functions that we are 
'*** looking for in the tokenization process
Public Class list_of_functions
    Private list(9) As String

    Public Sub New()
        list(0) = "sum"
        list(1) = "product"
        list(2) = "lim"
        list(3) = "integral"
        list(4) = "fraction"
        list(5) = "fraction2"
        list(6) = "exp2"
        list(7) = "sub2"
        list(8) = "lparen"
        list(9) = "rparen"
    End Sub

    '*** Looks through "list" and tries to see if the given
    '*** function_name exists in the list. Returns true if the
    '*** function_name is found. A linear search is used ---
    '*** there's only 10 items, so it shouldn't matter for now.
    Public Function find(ByVal function_name As String) As Boolean
        Dim i As Integer
        For i = 0 To list.Length - 1
            If String.Compare(function_name, list(i)) = 0 Then
                Return True
            End If
        Next
        Return False
    End Function

End Class
