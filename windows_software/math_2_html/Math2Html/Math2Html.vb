Public Class Math2Html

    '*** render_simple accepts a string that is the 
    '*** mathematical equation to be rendered. It 
    '*** will return the HTML representation of 
    '*** that mathematical equation.
    Public Shared Function render_simple(ByVal equation As String, ByVal symbols As Symbol_table) As String
        Dim return_val As String
        return_val = ""

        Dim token_array() As Token
        token_array = Token.tokenize(equation)

        'we build return_val by processing token_array one token at a time
        Dim i As Integer = 0
        For i = 0 To token_array.Length() - 1

            'tokens are of three types --- so there are
            'three cases on how to handle a token
            If token_array(i).token_type = TokenType.preformatted_text Then
                'this is the easiest case
                return_val = return_val + token_array(i).token_string

            ElseIf token_array(i).token_type = TokenType.normal_text Then
                'in this case, we have to do replacements so the
                'special symbols: &, <, <>, >, etc. renders properly
                Dim temp As String
                temp = format_normal_text(token_array(i).token_string)
                return_val = return_val + temp

            ElseIf token_array(i).token_type = TokenType.function_macro Then
                ' first determine if the token is a function or a macro
                Dim is_function As Boolean = False

                ' functions have parenthesis pairs: _sub(x)_

                ' find the location of the first parenthesis                
                ' first_paren = index location of the starting parenthsis
                Dim first_paren As Integer = -1
                first_paren = token_array(i).token_string.IndexOf("(")

                ' last_paren = index location of the ending parenthsis
                ' the last parenthesis should be at the second 
                ' last character of the string --- if the string
                ' is a function that is. Else this last_paren
                ' number has no meaning
                Dim last_paren As Integer = token_array(i).token_string.Length - 2

                If first_paren > 1 _
                And token_array(i).token_string.Chars(last_paren) = ")" Then
                    is_function = True                
                End If

                '*** at this point, we know whether the token
                '*** is_function or not

                If is_function Then

                    ' we need to identify the function name and function arguments
                    ' Then we call "format_function"

                    ' a function looks like: _root(x, y)_
                    ' so the function name is between index location 1 and the first '('
                    Dim token_string As String = token_array(i).token_string
                    Dim function_name As String = token_string.Substring(1, first_paren - 1)

                    ' the arguments string goes from right after the first_parenthesis
                    ' to the third last character
                    Dim arg_str_length As Integer = token_string.Length - 3 - first_paren
                    Dim arg_str = token_string.Substring(first_paren + 1, arg_str_length)

                    ' Now that we have the entire argument string, the individual
                    ' arguments are deliminated by commas. The argument
                    ' extraction class is in a helper function:
                    Dim arg_array() As String = get_arguments(arg_str)

                    return_val = return_val + format_function(function_name, arg_array, symbols)

                Else
                    'if token_array(i).token_string is not a function
                    'check to see if it's a macro
                    Dim is_macro As Boolean = False

                    Dim substitute_str As String
                    Dim token_string As String = token_array(i).token_string
                    substitute_str = symbols.look_up_macro(token_string)

                    If substitute_str.Length > 0 Then is_macro = True

                    If is_macro Then
                        return_val = return_val + substitute_str
                    Else
                        return_val = return_val + format_normal_text(token_string)
                    End If
                End If

            End If      '* This is the end of: 
            '* ElseIf token_array(i).token_type = TokenType.function_macro Then

        Next
      
        Return return_val
    End Function


    '*** This function replaces the special symbols &, <=, >=,
    '*** <>, !=, <, > so that the text provided will render
    '*** properly as html
    Private Shared Function format_normal_text(ByVal text As String) As String
        Dim temp As String = text
        temp = temp.Replace("&", "&amp;")
        temp = temp.Replace("<=", "&#8804;")
        temp = temp.Replace(">=", "&#8805;")
        temp = temp.Replace("<>", "&#8800;")
        temp = temp.Replace("!=", "&#8800;")
        temp = temp.Replace("<", "&lt;")
        temp = temp.Replace(">", "&gt;")
        Return temp
    End Function


    '*** This function takes in a string of arguments
    '*** and return the arguments as individual strings
    '*** in an array. Example input:
    '*** arg_string = "_func(a, b, c)_, (x, y), arg3"
    '*** return_val(0) = "_func(a, b, c)_"
    '*** return_val(1) = "(x, y)"
    '*** return_val(2) = "arg3"

    '*** If there are no arguments --- that arg_string is = ""
    '*** get_arguments will return: return_val(0) = ""
    Private Shared Function get_arguments(ByVal arg_string As String) As String()
        'create the return_val array
        Dim return_val(10) As String

        'we will build up the return_val array one by one
        'the return_val that we are currently building up is
        'return_val(j)
        Dim j As Integer = 0

        'allocate memory for the first argument string
        return_val(0) = New String("")

        'go over arg_string character by character
        Dim i As Integer
        For i = 0 To arg_string.Length - 1

            '** at this point in the program, return_val(j)
            '** should always be non-null. If j = 0, then
            '** return_val(j) = non-null because we allocated
            '** memory for it. Additionally, we allocate memory
            '** for return_val(j) whenever we advance j

            If arg_string.Chars(i) = "," Then
                '* comma detected --- next character will be
                '* part of a new argument
                j = j + 1

                'expand the return_val array if needed
                If j > return_val.Length - 1 Then
                    ReDim Preserve return_val(j + 10)
                End If

                'allocate memory for new argument
                return_val(j) = New String("")

            ElseIf arg_string.Chars(i) = "_" Then
                '* if we encounter an underscore, scan ahead to 
                '* the next underscore
                Dim new_i As Integer
                new_i = arg_string.IndexOf("_", i + 1)
                If new_i = -1 Then
                    new_i = arg_string.Length - 1
                End If
                return_val(j) = return_val(j) + arg_string.Substring(i, new_i - i + 1)
                i = new_i

            ElseIf arg_string.Chars(i) = "(" Then
                '* if we encounter a '(', scan ahead to 
                '* the next ')'
                Dim new_i As Integer
                new_i = arg_string.IndexOf(")", i + 1)
                If new_i = -1 Then
                    new_i = arg_string.Length - 1
                End If
                return_val(j) = return_val(j) + arg_string.Substring(i, new_i - i + 1)
                i = new_i

            Else
                '* normal case:
                return_val(j) = return_val(j) + arg_string.Chars(i)

            End If

        Next

        'resize return_val properly prior to returning it
        ReDim Preserve return_val(j)

        Return return_val
    End Function


    '*** This function generates an html representation
    '*** based on the function name provided and the
    '*** array of string arguments provided. Here, the word
    '*** "function" refers to what the user types in, such
    '*** as _root(...)_, _exp(...)_, _sub(...)_
    Private Shared Function format_function(ByVal function_name As String, ByVal arguments As String(), ByVal symbols As Symbol_table) As String
        Dim return_val As String
        return_val = ""

        If String.Compare(function_name, "root") = 0 Then
            ' _root(x, y)_
            ' renders as:
            ' <sup>x</sup>&#8730;
            ' <span style="text-decoration: overline">
            ' y</span>
            '
            ' x is assumed to be a basic integer so that
            ' is treated as preformatted text
            ' y is assumed to be a simple expression, so
            ' that gets rendered by render_simple
            '
            ' alternative form: _root(y)_
            ' in this case we don't have the "<sup>x</sup>"

            If arguments.Length >= 1 Then
                Dim x As String
                Dim y As String

                If arguments.Length = 1 Then
                    x = New String("")
                    y = arguments(0)
                ElseIf arguments.Length = 2 Then
                    x = arguments(0)
                    y = arguments(1)
                End If

                '** at this point, x and y have correct values

                If arguments.Length = 2 Then
                    return_val = "<sup>" + x + "</sup>"
                End If

                return_val = return_val + "&#8730;"
                return_val = return_val + "<span style=""text-decoration: overline"">"
                return_val = return_val + render_simple(y, symbols)
                return_val = return_val + "</span>" + ControlChars.NewLine

            End If
            '* end of handling _root(x, y)_


        ElseIf String.Compare(function_name, "exp") = 0 Then
            '_exp(x)_
            'rendered as:
            '<sup>x</sup>
            '
            'x is assumed to be a simple expression, so it
            'gets rendered by render_simple

            If arguments.Length >= 1 Then
                Dim x As String = arguments(0)

                return_val = "<sup>"
                return_val = return_val + render_simple(x, symbols)
                return_val = return_val + "</sup>"

            End If
            '* end of handling _exp(x)_


        ElseIf String.Compare(function_name, "sub") = 0 Then
            '_sub(x)_
            'rendered as:
            '<sub>x</sub>
            '
            'x is assumed to be a simple expression, so it
            'gets rendered by render_simple

            If arguments.Length >= 1 Then
                Dim x As String = arguments(0)

                return_val = "<sub>"
                return_val = return_val + render_simple(x, symbols)
                return_val = return_val + "</sub>"

            End If
            '* end of handling _sub(x)_

        ElseIf String.Compare(function_name, "over") = 0 Then
            '_over(x)_
            'rendered as:
            '<span style="text-decoration: overline">x</span>
            '
            'x is assumed to be a simple expression, so it
            'gets rendered by render_simple

            If arguments.Length >= 1 Then
                Dim x As String = arguments(0)

                return_val = "<span style=""text-decoration: overline"">"
                return_val = return_val + render_simple(x, symbols)
                return_val = return_val + "</span>"

            End If
            '* end of handling _over(x)_

        End If

        Return return_val
    End Function


    '*** In render_complex and the supporting format_function2,
    '*** certain html tags appear so many times that it's
    '*** best to put their code into their own function

    '*** this function returns a <table> tag with border=0
    '*** cellspacing=0, and a cellpadding that is specified by
    '*** the caller of this function
    Private Shared Function make_table_tag(ByVal cellpadding As Integer) As String
        'example return:
        '<table border="0" cellpadding="3" cellspacing="0">
        Dim return_val As String = "<table border=""0"" cellpadding="""
        return_val = return_val + cellpadding.ToString + """ cellspacing=""0"">"
        return_val = return_val + ControlChars.NewLine
        Return return_val
    End Function

    '*** this function returns <td nowrap align="center">
    Private Shared Function make_td_tag() As String
        Return "<td nowrap align=""center"">" + ControlChars.NewLine
    End Function

    '*** this function returns <font> tag where the font face, 
    '*** font size, and the font color are all specified by
    '*** the caller of this function
    Private Shared Function make_font_tag(ByVal face As String, ByVal size As Integer, ByVal color As String) As String
        ' example return:
        ' <font face="Times New Roman" size="7" color="#FF6600">
        Dim return_val As String = "<font face=""" + face
        return_val = return_val + """ size=""" + size.ToString
        return_val = return_val + """ color=""#" + color
        return_val = return_val + """>" + ControlChars.NewLine
        Return return_val
    End Function

    '*** this function returns a <div> tag that will push stuff
    '*** down by the amount that the argument specified
    Private Shared Function make_div_tag(ByVal down As Single) As String
        'example return value:
        '<div style="top: -0.9ex; position: relative">
        Dim return_val As String = "<div style=""top: "
        return_val = return_val + down.ToString + "ex; position: relative"">"
        return_val = return_val + ControlChars.NewLine
        Return return_val
    End Function

    '*** this function returns a <div> tag that will push stuff
    '*** down and to the left by the amounts that 
    '*** the arguments specified
    Private Shared Function make_div_tag(ByVal down As Single, ByVal left As Single) As String
        'example return value:
        '<div style="position: relative; left: -1.2ex; top: -0.6ex">
        Dim return_val As String = "<div style=""position: relative; left: "
        return_val = return_val + left.ToString + "ex; top: "
        return_val = return_val + down.ToString + "ex"">" + ControlChars.NewLine
        Return return_val
    End Function

    '*** render_complex accepts a string that is the 
    '*** mathematical equation to be rendered. It 
    '*** will return the HTML representation of 
    '*** that mathematical equation.
    '*** render_complex will call render_simple eventually.
    '
    '*** The difference between render_complex and render_simple
    '*** is that render_complex accepts more functions
    '*** than render_simple
    Public Shared Function render_complex(ByVal equation As String, _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim return_val As String
        return_val = ""

        ' some variables frequently used in formatting return_val
        Dim indent As String = New String("    ")
        Dim face As String = user_settings_obj.font_face
        Dim color As String = user_settings_obj.font_color
        Dim size As String = user_settings_obj.font_size

        Dim token_array() As Token2
        token_array = Token2.tokenize(equation)

        '"equation" could be just one simple expression
        If token_array.Length = 1 _
            And token_array(0).token_type = TokenType2.simple_expression Then
            ' we want the following appended to return_val:
            ' <font face="Courier New" color="#FF6600" size="4">
            ' output from Math2Html.render_simple(...)
            ' </font>
            return_val = return_val + indent + make_font_tag(face, size, color)
            return_val = return_val + indent + Math2Html.render_simple(token_array(0).token_string, symbols)
            return_val = return_val + indent + "</font>" + ControlChars.NewLine
            Return return_val
        End If

        'return_val is a table with one row of cells
        return_val = return_val + make_table_tag(user_settings_obj.cell_padding)
        return_val = return_val + "<tr>" + ControlChars.NewLine

        'we build return_val by processing token_array one token at a time
        Dim i As Integer = 0
        For i = 0 To token_array.Length() - 1

            'each iteration through the for loop is one cell in the row
            return_val = return_val + make_td_tag()

            If token_array(i).token_type = TokenType2.simple_expression Then

                ' we want the following appended to return_val:
                ' <font face="Courier New" color="#FF6600" size="4">
                ' output from Math2Html.render_simple(...)
                ' </font>
                return_val = return_val + indent + make_font_tag(face, size, color)
                return_val = return_val + indent + Math2Html.render_simple(token_array(i).token_string, symbols)
                return_val = return_val + indent + "</font>" + ControlChars.NewLine

            ElseIf token_array(i).token_type = TokenType2.a_function Then
                ' use format_function2 to render the text
                ' In this case, format_function2 will generate
                ' the font tags

                'left_paren = the index location of the first '('
                Dim left_paren As Integer = token_array(i).token_string.IndexOf("(")

                Dim function_name As String
                function_name = token_array(i).token_string.Substring(1, left_paren - 1)

                Dim last_index As Integer = token_array(i).token_string.Length - 1

                Dim arg_string As String
                Dim arg_array() As String
                arg_string = token_array(i).token_string.Substring(left_paren + 1, last_index - 2 - left_paren)
                arg_array = get_arguments(arg_string)

                return_val = return_val + indent + format_function2(function_name, arg_array, symbols, user_settings_obj)

            End If

            return_val = return_val + "</td>" + ControlChars.NewLine
        Next

        return_val = return_val + "</tr></table>" + ControlChars.NewLine + ControlChars.NewLine
        Return return_val

    End Function


    '*** format_function gets called by render_simple
    '*** format_function2 gets called by render_simple2
    '*** This function generates an html representation
    '*** based on the function name provided, the
    '*** array of string arguments provided, and the 
    '*** numbers in user_settings_obj. Here, the word
    '*** "function" refers to what the user types in, such
    '*** as _lim(...)_, _sum(...)_, etc.
    Private Shared Function format_function2(ByVal function_name As String, _
    ByVal arguments As String(), ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim return_val As String
        return_val = ""

        If String.Compare(function_name, "sum") = 0 Then
            return_val = format_sum(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "product") = 0 Then
            return_val = format_product(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "lim") = 0 Then
            return_val = format_lim(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "integral") = 0 Then
            return_val = format_integral(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "fraction") = 0 Then
            return_val = format_fraction(1, arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "fraction2") = 0 Then
            return_val = format_fraction(3, arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "exp2") = 0 Then
            return_val = format_exp2(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "sub2") = 0 Then
            return_val = format_sub2(arguments, symbols, user_settings_obj)

        ElseIf String.Compare(function_name, "lparen") = 0 Then
            '<font...>(</font>
            return_val = make_font_tag( _
                user_settings_obj.lparen_font_face, _
                user_settings_obj.lparen_font_size, _
                user_settings_obj.font_color)
            return_val += "(</font>" + ControlChars.NewLine

        ElseIf String.Compare(function_name, "rparen") = 0 Then
            '<font...>)</font>
            return_val = make_font_tag( _
                user_settings_obj.rparen_font_face, _
                user_settings_obj.rparen_font_size, _
                user_settings_obj.font_color)
            return_val += ")</font>" + ControlChars.NewLine

        End If

        Return return_val

    End Function


    '*** These next functions are all called by format_function2.
    '*** format_function2 basically detects what needs to be
    '*** formatted, and then it diverges to one of these
    '*** functions to perform the formatting. This is necessary
    '*** to keep the code in format_function2 from being too long


    Private Shared Function format_sum(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim indent As String = "        "
        Dim return_val As String = ""

        'here is the easy case
        If arguments.Length = 1 And arguments(0).Length = 0 Then
            'just return the sigma symbol in extra large font
            return_val = make_font_tag( _
                user_settings_obj.sum_symbol_font_face, _
                user_settings_obj.sum_symbol_font_size, _
                user_settings_obj.font_color)
            return_val = return_val + "&#931;</font>" + ControlChars.NewLine
            Return return_val
        End If

        'the general case: _sum(x, y)_
        'code we want:
        '<table ...>
        '<tr><td ...>
        '    <div top: 1.5ex ...>
        '    <font ...>x</font></div>
        '</td></tr>
        '<tr><td ...>
        '    <font face="Times New Roman" size="7" ...>&#931;</font>
        '</td></tr>
        '<tr><td ...>
        '    <div top: -1.4ex ...>
        '    <font ...>y</font></div>
        '</td></tr>
        '</table>

        Dim x As String
        Dim y As String

        If arguments.Length = 1 Then
            'note, in this case, x needs to be made into 
            'preformatted text because later I will use
            'render_simple to render x and y --- the preformatted
            'text keeps render_simple from processing x.
            x = New String("""&#8734;""")
            y = arguments(0)
        ElseIf arguments.Length = 2 Then
            x = arguments(0)
            y = arguments(1)
        End If

        '<table ...>
        '<tr><td ...>
        return_val = make_table_tag(0)
        return_val = return_val + "<tr>" + make_td_tag()

        '    <div top: 1.5ex ...>
        return_val = return_val + indent
        If arguments.Length >= 2 Then
            return_val = return_val + make_div_tag(user_settings_obj.sum_row1_down)
        ElseIf arguments.Length = 1 Then
            return_val = return_val + make_div_tag(user_settings_obj.sum_row1_down2)
        End If

        '    <font ...>x</font></div>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.sum_arg_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + render_simple(x, symbols)
        return_val = return_val + "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <font face="Times New Roman" size="7" ...>&#931;</font>
        return_val = return_val + indent + make_font_tag( _
                user_settings_obj.sum_symbol_font_face, _
                user_settings_obj.sum_symbol_font_size, _
                user_settings_obj.font_color)
        return_val = return_val + "&#931;</font>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <div top: -1.4ex ...>
        return_val = return_val + indent
        return_val = return_val + make_div_tag(user_settings_obj.sum_row3_down)

        '    <font ...>y</font></div>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.sum_arg_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + render_simple(y, symbols)
        return_val = return_val + "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '</table>
        return_val = return_val + "</td></tr></table>" + ControlChars.NewLine

        Return return_val
    End Function


    ' note: format_product is very similar to format_sum
    ' the only difference is to use different variables
    ' from user_settings_obj, and to have the product symbol
    ' rather than the sigma symbol
    ' the comments are left as they are, talking about "sum"
    ' to emphasize the fact that this function is basically
    ' copied from the "sum" function

    Private Shared Function format_product(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim indent As String = "        "
        Dim return_val As String = ""

        'here is the easy case
        If arguments.Length = 1 And arguments(0).Length = 0 Then
            'just return the sigma symbol in extra large font
            return_val = make_font_tag( _
                user_settings_obj.product_symbol_font_face, _
                user_settings_obj.product_symbol_font_size, _
                user_settings_obj.font_color)
            return_val = return_val + "&#8719;</font>" + ControlChars.NewLine
            Return return_val
        End If

        'the general case: _sum(x, y)_
        'code we want:
        '<table ...>
        '<tr><td ...>
        '    <div top: 1.5ex ...>
        '    <font ...>x</font></div>
        '</td></tr>
        '<tr><td ...>
        '    <font face="Times New Roman" size="7" ...>&#931;</font>
        '</td></tr>
        '<tr><td ...>
        '    <div top: -1.4ex ...>
        '    <font ...>y</font></div>
        '</td></tr>
        '</table>

        Dim x As String
        Dim y As String

        If arguments.Length = 1 Then
            'note, in this case, x needs to be made into 
            'preformatted text because later I will use
            'render_simple to render x and y --- the preformatted
            'text keeps render_simple from processing x.
            x = New String("""&#8734;""")
            y = arguments(0)
        ElseIf arguments.Length = 2 Then
            x = arguments(0)
            y = arguments(1)
        End If

        '<table ...>
        '<tr><td ...>
        return_val = make_table_tag(0)
        return_val = return_val + "<tr>" + make_td_tag()

        '    <div top: 1.5ex ...>
        return_val = return_val + indent
        If arguments.Length >= 2 Then
            return_val = return_val + make_div_tag(user_settings_obj.product_row1_down)
        ElseIf arguments.Length = 1 Then
            return_val = return_val + make_div_tag(user_settings_obj.product_row1_down2)
        End If

        '    <font ...>x</font></div>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.product_arg_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + render_simple(x, symbols)
        return_val = return_val + "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <font face="Times New Roman" size="7" ...>&#931;</font>
        return_val = return_val + indent + make_font_tag( _
                user_settings_obj.product_symbol_font_face, _
                user_settings_obj.product_symbol_font_size, _
                user_settings_obj.font_color)
        return_val = return_val + "&#8719;</font>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <div top: -1.4ex ...>
        return_val = return_val + indent
        return_val = return_val + make_div_tag(user_settings_obj.product_row3_down)

        '    <font ...>y</font></div>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.product_arg_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + render_simple(y, symbols)
        return_val = return_val + "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '</table>
        return_val = return_val + "</td></tr></table>" + ControlChars.NewLine

        Return return_val
    End Function

    Private Shared Function format_lim(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim return_val As String = ""
        Dim indent As String = "        "

        'return value =
        '<table ...>
        '<tr><td ...>
        '    <font ...>lim</font>
        '</td></tr>
        '<tr><td ...>
        '    <div style="top: -0.9ex; ...>
        '    <font ...>x</font>
        '    </div>
        '</td></tr>
        '</table>

        '<table ...>
        '<tr><td ...>
        return_val = make_table_tag(0) + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <font ...>lim</font>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.lim_symbol_font_face, _
            user_settings_obj.lim_symbol_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + "lim</font>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "<tr>" + make_td_tag() + ControlChars.NewLine

        '    <div style="top: -0.9ex; ...>
        return_val = return_val + indent + make_div_tag(user_settings_obj.lim_row2_down)

        '    <font ...>x</font>
        return_val = return_val + indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.lim_arg_font_size, _
            user_settings_obj.font_color)
        return_val = return_val + render_simple(arguments(0), symbols)
        return_val = return_val + "</font>" + ControlChars.NewLine

        '    </div>
        '</td></tr>
        '</table>
        return_val = return_val + indent + "</div>" + ControlChars.NewLine
        return_val = return_val + "</td></tr>" + ControlChars.NewLine
        return_val = return_val + "</table>" + ControlChars.NewLine

        Return return_val
    End Function

    Private Shared Function format_integral(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim return_val As String = ""
        Dim indent As String = "        "

        'the easier, zero argument case first
        If arguments(0).Length = 0 Then
            'return <font ...>&#8747;</font>
            return_val = indent + make_font_tag( _
                user_settings_obj.integral_symbol_font_face, _
                user_settings_obj.integral_symbol_font_size, _
                user_settings_obj.font_color)
            return_val = return_val + "&#8747;</font>" + ControlChars.NewLine

            Return return_val
        End If

        'return value for _integral(x, y)_
        '<table ...>
        '<tr><td nowrap align="center" rowspan="2">
        '    <font ...>&#8747;</font>
        '</td>
        '<td ...>
        '    <div left: -1.2ex; top: -0.6ex ...">
        '    <font ...>x</font></div>
        '</td></tr>
        '<tr><td ...>
        '    <div left: -1.2ex; top: 0.1ex ...">
        '    <font ...>y</font></div>
        '</td></tr>
        '</table>

        If arguments.Length < 2 Then Return ""

        '*** at this point, we already left the function if
        '*** the argument array length is less than 2

        Dim x As String = arguments(0)
        Dim y As String = arguments(1)

        '<table ...>
        '<tr><td nowrap align="center" rowspan="2">
        return_val = make_table_tag(0)
        'don't use the make_td_tag() to render the td here!
        'because the <td> tag here has rowspan="2"
        return_val += "<tr><td nowrap align=""center"" rowspan=""2"">" + ControlChars.NewLine

        '    <font ...>&#8747;</font>
        return_val += indent + make_font_tag( _
            user_settings_obj.integral_symbol_font_face, _
            user_settings_obj.integral_symbol_font_size, _
            user_settings_obj.font_color)
        return_val += "&#8747;</font>" + ControlChars.NewLine

        '</td>
        '<td ...>
        'don't use make_td_tag because we don't want the limits of
        'integration to align to the center of the table cell
        return_val += "</td>" + ControlChars.NewLine
        return_val += "<td nowrap>" + ControlChars.NewLine

        '    <div left: -1.2ex; top: -0.6ex ...">
        return_val += make_div_tag( _
            user_settings_obj.integral_row1_down, _
            user_settings_obj.integral_row1_left)

        '    <font ...>x</font></div>
        return_val += make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.integral_arg_font_size, _
            user_settings_obj.font_color)
        return_val += render_simple(x, symbols)
        return_val += "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td ...>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "<tr>" + "<td nowrap>" + ControlChars.NewLine

        '    <div left: -1.2ex; top: 0.1ex ...">
        return_val += make_div_tag( _
            user_settings_obj.integral_row2_down, _
            user_settings_obj.integral_row2_left)

        '    <font ...>y</font></div>
        return_val += make_font_tag( _
                    user_settings_obj.font_face, _
                    user_settings_obj.integral_arg_font_size, _
                    user_settings_obj.font_color)
        return_val += render_simple(y, symbols)
        return_val += "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '</table>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "</table>" + ControlChars.NewLine

        Return return_val
    End Function


    'one format_fraction handles both _fraction(x, y)_ and
    '_fraction2(x, y)_ because they differ only in the width
    'of the horizontal rule

    Private Shared Function format_fraction(ByVal line_width As Integer, _
    ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        Dim return_val As String = ""
        Dim indent As String = "        "

        'return value:
        '<table ...>
        '<tr><td...>
        '    <div top: +0.7ex ...>
        '    <font ...>x</font></div>
        '</td></tr>
        '<tr><td...>
        '    <hr color="#FF6600" size="1">
        '</td></tr>
        '<tr><td...>
        '    <div top: -0.7ex ...>
        '    <font ...>y</font></div>
        '</td></tr>
        '</table>

        If arguments.Length < 2 Then Return ""

        '*** at this point, we can assume that the arguments
        '*** array has at least 2 strings

        Dim x As String = arguments(0)
        Dim y As String = arguments(1)

        '<table ...>
        '<tr><td...>
        return_val = make_table_tag(0)
        return_val += "<tr>" + make_td_tag()

        '    <div top: +0.7ex ...>
        return_val += indent + make_div_tag(user_settings_obj.fraction_row1_down)

        '    <font ...>x</font></div>
        ' actually we let reder_complex render x --- we do not
        ' give it a font tag
        return_val += indent + render_complex(x, symbols, user_settings_obj)
        return_val += "</div>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td...>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "<tr>" + make_td_tag()

        '    <hr color="#FF6600" size="1">
        return_val += "<hr color=""#" + user_settings_obj.font_color
        return_val += """ size=""" + line_width.ToString + """>"

        '</td></tr>
        '<tr><td...>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "<tr>" + make_td_tag()

        '    <div top: -0.7ex ...>
        return_val += indent + make_div_tag(user_settings_obj.fraction_row3_down)

        '    <font ...>y</font></div>
        ' actually we let reder_complex render y --- we do not
        ' give it a font
        return_val += indent + render_complex(y, symbols, user_settings_obj)
        return_val += "</div>" + ControlChars.NewLine

        '</td></tr>
        '</table>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "</table>" + ControlChars.NewLine

        Return return_val
    End Function

    Private Shared Function format_exp2(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        'output:
        '<table ...>
        '<tr><td ...>&nbsp;</td>
        '<td>
        '    <div top: 1.6ex ...>
        '    <font ...>y</font></div>
        '</td></tr>
        '<tr><td>
        '    <font ...>x</font>
        '</td><td>&nbsp;</td></tr>
        '</table>

        If arguments.Length < 2 Then Return ""

        '** at this point, we can safely assume arguments array 
        '** has at length of 2 at least
        Dim x As String = arguments(0)
        Dim y As String = arguments(1)

        Dim return_val As String = ""
        Dim indent As String = "        "

        '<table ...>
        '<tr><td ...>&nbsp;</td>
        '<td>
        return_val = make_table_tag(0)
        return_val += "<tr>" + make_td_tag() + "&nbsp;</td>" + ControlChars.NewLine
        return_val += "<td>" + ControlChars.NewLine

        '    <div top: 1.6ex ...>
        return_val += indent + make_div_tag(user_settings_obj.exponent_down)

        '    <font ...>y</font></div>
        return_val += indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.exponent_font_size, _
            user_settings_obj.font_color)
        return_val += render_simple(y, symbols)
        return_val += "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '<tr><td>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "<tr><td>" + ControlChars.NewLine

        '    <font ...>x</font>
        ' x is rendered by render_complex, so we do not actually
        ' provide a font tag here
        return_val += indent + render_complex(x, symbols, user_settings_obj)

        '</td><td>&nbsp;</td></tr>
        '</table>
        return_val += "</td><td>&nbsp;</td></tr>" + ControlChars.NewLine
        return_val += "</table>" + ControlChars.NewLine

        Return return_val
    End Function

    Private Shared Function format_sub2(ByVal arguments As String(), _
    ByVal symbols As Symbol_table, ByVal user_settings_obj As User_settings) As String
        'output:
        '<table ...>
        '<tr><td ...>
        '    <font ...>x</font>
        '</td><td>&nbsp;</td></tr>
        '<tr><td>&nbsp;</td><td...>
        '    <div ...>
        '    <font ...>y</font></div>
        '</td></tr>
        '</table>

        If arguments.Length < 2 Then Return ""

        '** at this point, we can safely assume arguments array 
        '** has at length of 2 at least
        Dim x As String = arguments(0)
        Dim y As String = arguments(1)

        Dim return_val As String = ""
        Dim indent As String = "     "

        '<table ...>
        '<tr><td ...>
        return_val = make_table_tag(0)
        return_val += "<tr>" + make_td_tag()

        '    <font ...>x</font>
        ' x is generated by render_complex --- so we don't really
        ' generate the <font> tag
        return_val += indent + render_complex(x, symbols, user_settings_obj)

        '</td><td>&nbsp;</td></tr>
        '<tr><td>&nbsp;</td><td...>
        return_val += "</td><td>&nbsp;</td></tr>" + ControlChars.NewLine
        return_val += "<tr><td>&nbsp;</td>" + make_td_tag()

        '    <div ...>
        return_val += indent + make_div_tag(user_settings_obj.subscript_down)

        '    <font ...>y</font></div>
        return_val += indent + make_font_tag( _
            user_settings_obj.font_face, _
            user_settings_obj.subscript_font_size, _
            user_settings_obj.font_color)
        return_val += render_simple(y, symbols)
        return_val += "</font></div>" + ControlChars.NewLine

        '</td></tr>
        '</table>
        return_val += "</td></tr>" + ControlChars.NewLine
        return_val += "</table>" + ControlChars.NewLine

        Return return_val
    End Function
End Class
