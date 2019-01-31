'*** this object contains information on how the user
'*** want the mathematical equations to be rendered
Public Class User_settings
    Public font_face As String = "Courier New"
    Public font_color As String = "FF8000"
    Public font_size As Integer = 5

    Public style_sheet_on As Boolean = True

    'This number is used in render_complex. It determines
    'how far apart are the elements rendered in adjacent cells
    Public cell_padding As Integer = 6

    'for formatting _sum(x, y)_
    Public sum_symbol_font_face As String = "Times New Roman"
    Public sum_symbol_font_size As Integer = 7
    Public sum_arg_font_size As Integer = 3
    Public sum_row1_down As Single = 1.5
    Public sum_row1_down2 As Single = 1.8
    Public sum_row3_down As Single = -1.4

    'for formatting _product(x, y)_
    Public product_symbol_font_face As String = "Times New Roman"
    Public product_symbol_font_size As Integer = 7
    Public product_arg_font_size As Integer = 3
    Public product_row1_down As Single = 0.8
    Public product_row1_down2 As Single = 1.2
    Public product_row3_down As Single = -1.4

    'for formatting _lim(x)_
    Public lim_symbol_font_face As String = "Times New Roman"
    Public lim_symbol_font_size As Integer = 6
    Public lim_arg_font_size As Integer = 3
    Public lim_row2_down As Single = -0.9

    'for formatting _integral(x, y)_
    Public integral_symbol_font_face As String = "Times New Roman"
    Public integral_symbol_font_size As Integer = 7
    Public integral_arg_font_size As Integer = 3
    Public integral_row1_left As Single = -1.2
    Public integral_row1_down As Single = -0.6
    Public integral_row2_left As Single = -1.2
    Public integral_row2_down As Single = 0.1

    'for formatting _fraction(x, y)_ and _fraction2(x, y)_
    Public fraction_row1_down As Single = 0.7
    Public fraction_row3_down As Single = -0.7

    'for formatting _exp2(x, y)_
    Public exponent_down As Single = 1.6
    Public exponent_font_size As Integer = 3

    'for formatting _sub2(x, y)_
    Public subscript_down As Single = -1.7
    Public subscript_font_size As Integer = 3

    'for formatting _lparen()_
    Public lparen_font_face As String = "Courier New"
    Public lparen_font_size As Integer = 7

    'for formatting _rparen()_
    Public rparen_font_face As String = "Courier New"
    Public rparen_font_size As Integer = 7

End Class
