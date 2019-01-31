'*** This class will break a user input string into
'*** tokens. The user input string is some kind of mathematical
'*** expression and the tokens are substrings of the user input.
'*** Each token(substring) is of a certain "type" and this type
'*** field tells the main program how to parse the token

Public Class Token
    Public token_string As String
    Public token_type As TokenType

    Public Sub New()
        token_string = New String("")
        token_type = TokenType.normal_text
    End Sub

    '*** this function takes in a string and generates an array of
    '*** Tokens. See documentation for parsing rules.
    '*** example:   
    '*** input = 
    '***    normal text some normal, text "preformatted stuff _a macro_",  _somemacro_ normal text _a function("preformatted stuff")_
    '*** return value = 
    '***    token_array.length	6
    '***
    '***    token_array(0).token_string	"normal text some normal, text "
    '***    token_array(1).token_string	"preformatted stuff _a macro_"
    '***    token_array(2).token_string	",  "
    '***    token_array(3).token_string	"_somemacro_"
    '***    token_array(4).token_string	" normal text "
    '***    token_array(5).token_string	"_a function("preformatted stuff")_"
    '***
    '***    token_array(0).token_type	normal_text
    '***    token_array(1).token_type	preformatted_text
    '***    token_array(2).token_type	normal_text
    '***    token_array(3).token_type	function_macro
    '***    token_array(4).token_type	normal_text
    '***    token_array(5).token_type	function_macro
    Public Shared Function tokenize(ByVal math_expression As String) As Token()
        Dim return_val(10) As Token

        ' Rightnow all 11 return_val elements are empty
        ' We will fill in return_val(0) first, then
        ' return_val(1) and so forth. The index "j" denotes
        ' which return_val element we are on. At the end, we
        ' will resize return_val so there are no "empty" 
        ' return_val elements
        Dim j As Integer = 0

        ' allocate memory for item zero
        return_val(0) = New Token()
        return_val(0).token_type = TokenType.normal_text

        ' go over the entire string character by character
        Dim i As Integer = 0
        For i = 0 To math_expression.Length - 1

            '** at this point in the program, return_val(j)
            '** should always be non-null. If j = 0, then
            '** return_val(j) = non-null because we allocated
            '** memory for it. Additionally, we allocate memory
            '** for return_val(j) whenever we advance j

            'Each iteration of for-loop has 3 possibilities,
            '1 possibility for each type of tokens

            If math_expression.Chars(i) = ControlChars.Quote Then
                'if the character is a quote, then we ran into a
                'preformatted text section. We look for the ending
                'quote to find the end of the preformatted section
                Dim end_index As Integer = -1
                end_index = math_expression.IndexOf(ControlChars.Quote, i + 1)

                If end_index = -1 Then
                    '* If there is no end quote found,
                    '* let the remainder of the string be preformatted text
                    ' and the final "quote", which never existed, should be
                    ' at one character beyond the end of the current
                    ' string
                    end_index = math_expression.Length
                End If

                'we don't want to put the starting and ending
                'quotes into return_val(j)
                Dim length = end_index - i - 1

                'this block adds the preformatted text to the token
                If length > 0 Then
                    ' #1. We might have to create a new token to
                    ' hold the current preformatted text segment
                    ' #2. Then afterward we have to create a second
                    ' new token as preparation for the next loop

                    'start a new token if necessary (#1)
                    If return_val(j).token_string.Length > 0 Then
                        j = j + 1
                        ' enlarge return_val if necessary
                        If j > return_val.Length - 1 Then
                            ReDim Preserve return_val(j + 10)
                        End If
                        return_val(j) = New Token()
                    End If

                    return_val(j).token_string = math_expression.Substring(i + 1, length)
                    return_val(j).token_type = TokenType.preformatted_text

                    ' now for the #2 part --- preparation for the
                    ' next loop
                    If end_index < math_expression.Length - 1 Then
                        j = j + 1
                        ' enlarge return_val if necessary
                        If j > return_val.Length - 1 Then
                            ReDim Preserve return_val(j + 10)
                        End If
                        return_val(j) = New Token()                        
                    End If

                End If

                'on the next iteration of the for loop, we need to
                'be at the spot i = end_index + 1
                i = end_index

                '* end of the block that handles preformatted text


            ElseIf math_expression.Chars(i) = "_" Then

                'if the character is an underscore, then we ran into a
                'function/macro section. We look for the ending
                'underscore to find the end of the this section
                Dim end_index As Integer = -1

                ' the following won't work:
                ' end_index = math_expression.IndexOf("_", i + 1)

                ' simply looking for the next underscore works
                ' if the expression is a macro, but might not
                ' work if the expression is a function
                ' example:
                ' we can have expression = 
                '   -b_plusminus_ _root( , b_exp(2)_ - 4ac)_
                ' where the _root_ function contains within it
                ' another function. We want the entire:
                ' _root( , b_exp(2)_ - 4ac)_
                ' to be one token.

                end_index = find_next_underscore(math_expression, i + 1)

                If end_index = -1 Then
                    '* If there is no end underscore found, let this
                    '* underscore character be part of the current "normal" text token
                    return_val(j).token_string = return_val(j).token_string + "_"
                    end_index = i
                Else

                    ' we do want the starting and ending '_'
                    ' in return_val(j)
                    Dim length = end_index - i + 1

                    If length = 0 Then
                        'we have two underscores in a row
                        return_val(j).token_string = return_val(j).token_string + "__"
                    End If

                    'this block adds the function_macro text to the token
                    If length > 0 Then
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
                            return_val(j) = New Token()
                        End If

                        return_val(j).token_string = math_expression.Substring(i, length)
                        return_val(j).token_type = TokenType.function_macro

                        ' now for the #2 part --- preparation for the
                        ' next loop
                        If end_index < math_expression.Length - 1 Then
                            j = j + 1
                            ' enlarge return_val if necessary
                            If j > return_val.Length - 1 Then
                                ReDim Preserve return_val(j + 10)
                            End If
                            return_val(j) = New Token()
                        End If

                    End If

                End If

                'on the next iteration of the for loop, we need to
                'be at the spot i = end_index + 1
                i = end_index

                '* end of the block that handles function/macro text

            Else
                ' default case --- add the current character to the
                ' current return_val object
                return_val(j).token_string = return_val(j).token_string + math_expression.Chars(i)
                ' no need for:
                ' return_val(j).token_type = TokenType.normal_text
                ' because it's in the constructor already
            End If

        Next

        'resize the return_val array so the caller of this
        'function will receiver an array without "empty" 
        'elements at the end of the array       
        ReDim Preserve return_val(j)

        Return return_val
    End Function


    '*** This function looks for the next underscore taking into
    '*** account the situation we face in this program --- 
    '*** ensuring matching parenthesis pairs
    '*** If no underscore is found, the function returns -1
    Private Shared Function find_next_underscore(ByVal math_expression As String, ByVal start_index As Integer) As Integer
        '*** given math_expression = 
        '   -b_plusminus__root( , b_exp(2)_ - 4ac)_
        '*** we want the third token to be:
        '*** _root( , b_exp(2)_ - 4ac)_
        '*** not just
        '*** _root( , b_
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

Public Enum TokenType
    normal_text = 0
    preformatted_text = 1
    function_macro = 2
End Enum
