Imports System.IO

Public Class Form1
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    Friend WithEvents User_input_box As System.Windows.Forms.TextBox
    Friend WithEvents Render_button As System.Windows.Forms.Button
    Friend WithEvents Copy_to_clipboard_button As System.Windows.Forms.Button
    Friend WithEvents MainMenu1 As System.Windows.Forms.MainMenu
    Friend WithEvents MenuItem1 As System.Windows.Forms.MenuItem
    Friend WithEvents Change_color_menu_item As System.Windows.Forms.MenuItem
    Friend WithEvents AxWebBrowser1 As AxSHDocVw.AxWebBrowser
    Friend WithEvents ColorDialog1 As System.Windows.Forms.ColorDialog
    Friend WithEvents MenuItem3 As System.Windows.Forms.MenuItem
    Friend WithEvents Help_menu_item As System.Windows.Forms.MenuItem
    Friend WithEvents Change_offsets_menu As System.Windows.Forms.MenuItem
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.User_input_box = New System.Windows.Forms.TextBox()
        Me.Copy_to_clipboard_button = New System.Windows.Forms.Button()
        Me.Render_button = New System.Windows.Forms.Button()
        Me.MainMenu1 = New System.Windows.Forms.MainMenu(Me.components)
        Me.MenuItem1 = New System.Windows.Forms.MenuItem()
        Me.Change_color_menu_item = New System.Windows.Forms.MenuItem()
        Me.Change_offsets_menu = New System.Windows.Forms.MenuItem()
        Me.MenuItem3 = New System.Windows.Forms.MenuItem()
        Me.Help_menu_item = New System.Windows.Forms.MenuItem()
        Me.AxWebBrowser1 = New AxSHDocVw.AxWebBrowser()
        Me.ColorDialog1 = New System.Windows.Forms.ColorDialog()
        CType(Me.AxWebBrowser1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'User_input_box
        '
        Me.User_input_box.Anchor = CType(((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.User_input_box.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.User_input_box.Location = New System.Drawing.Point(13, 12)
        Me.User_input_box.Name = "User_input_box"
        Me.User_input_box.Size = New System.Drawing.Size(439, 35)
        Me.User_input_box.TabIndex = 0
        '
        'Copy_to_clipboard_button
        '
        Me.Copy_to_clipboard_button.Anchor = CType(((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Copy_to_clipboard_button.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Copy_to_clipboard_button.Location = New System.Drawing.Point(164, 338)
        Me.Copy_to_clipboard_button.Name = "Copy_to_clipboard_button"
        Me.Copy_to_clipboard_button.Size = New System.Drawing.Size(270, 46)
        Me.Copy_to_clipboard_button.TabIndex = 1
        Me.Copy_to_clipboard_button.Text = "Copy Html to Clipboard"
        '
        'Render_button
        '
        Me.Render_button.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.Render_button.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Render_button.Location = New System.Drawing.Point(477, 12)
        Me.Render_button.Name = "Render_button"
        Me.Render_button.Size = New System.Drawing.Size(120, 35)
        Me.Render_button.TabIndex = 3
        Me.Render_button.Text = "Render"
        '
        'MainMenu1
        '
        Me.MainMenu1.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.MenuItem1})
        '
        'MenuItem1
        '
        Me.MenuItem1.Index = 0
        Me.MenuItem1.MenuItems.AddRange(New System.Windows.Forms.MenuItem() {Me.Change_color_menu_item, Me.Change_offsets_menu, Me.MenuItem3, Me.Help_menu_item})
        Me.MenuItem1.Text = "&Options"
        '
        'Change_color_menu_item
        '
        Me.Change_color_menu_item.Index = 0
        Me.Change_color_menu_item.Text = "Change Color"
        '
        'Change_offsets_menu
        '
        Me.Change_offsets_menu.Index = 1
        Me.Change_offsets_menu.Text = "Change Style Sheet Offsets"
        '
        'MenuItem3
        '
        Me.MenuItem3.Index = 2
        Me.MenuItem3.Text = "-"
        '
        'Help_menu_item
        '
        Me.Help_menu_item.Index = 3
        Me.Help_menu_item.Shortcut = System.Windows.Forms.Shortcut.F1
        Me.Help_menu_item.Text = "Help"
        '
        'AxWebBrowser1
        '
        Me.AxWebBrowser1.Anchor = CType((((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Bottom) _
            Or System.Windows.Forms.AnchorStyles.Left) _
            Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.AxWebBrowser1.Enabled = True
        Me.AxWebBrowser1.Location = New System.Drawing.Point(26, 70)
        Me.AxWebBrowser1.OcxState = CType(resources.GetObject("AxWebBrowser1.OcxState"), System.Windows.Forms.AxHost.State)
        Me.AxWebBrowser1.Size = New System.Drawing.Size(571, 244)
        Me.AxWebBrowser1.TabIndex = 4
        '
        'ColorDialog1
        '
        Me.ColorDialog1.Color = System.Drawing.Color.FromArgb(CType(CType(255, Byte), Integer), CType(CType(128, Byte), Integer), CType(CType(0, Byte), Integer))
        '
        'Form1
        '
        Me.AcceptButton = Me.Render_button
        Me.AutoScaleBaseSize = New System.Drawing.Size(8, 19)
        Me.ClientSize = New System.Drawing.Size(618, 395)
        Me.Controls.Add(Me.AxWebBrowser1)
        Me.Controls.Add(Me.Render_button)
        Me.Controls.Add(Me.Copy_to_clipboard_button)
        Me.Controls.Add(Me.User_input_box)
        Me.Menu = Me.MainMenu1
        Me.Name = "Form1"
        Me.Text = "Math2Html"
        CType(Me.AxWebBrowser1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

#End Region

    '*** every time the user clicks the render button, we 
    '*** render the equation in User_input_box as an html
    '*** string. This string will be stored in the variable
    '*** html_format
    Private html_format As String = ""

    '*** this object tracks the user settings on how the
    '*** user wants the mathematical expressions rendered
    Private user_settings_obj As User_settings

    '*** this object stores all the macros available to the
    '*** program. Macros are (user_input, html_code_render)
    '*** pairs. Example (_pd_, &#8706;). Symbol_table_obj
    '*** stores a bunch of these pairs and allows the main
    '*** program to look up "_pd_" and retrieve "&#8706;"
    Private symbol_table_obj As Symbol_table

    '*** This is the dialog window that allows us to tweak
    '*** how fractions will be rendered
    Private offset_dialog As EditOffsets

    Private Sub Render_button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Render_button.Click

        html_format = Math2Html.render_complex(User_input_box.Text, _
        symbol_table_obj, user_settings_obj)

        '** At this point, html_format is the representation
        '** of the equation that the user entered

        '* We write this HTML code to a file
        '* then we direct the browser control to open this
        '* file

        'prepare a writer stream "sw" that we can write to
        Dim MyPath As String
        MyPath = CurDir()
        'MyPath is the current directory
        Dim fs As FileStream
        fs = New FileStream(MyPath + "\test.htm", FileMode.Create, FileAccess.Write)
        Dim sw As StreamWriter
        sw = New StreamWriter(fs)

        sw.WriteLine(html_format)
        sw.Close()

        AxWebBrowser1.Navigate2(MyPath + "\test.htm")
    End Sub

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        symbol_table_obj = New Symbol_table("macros.txt")
        user_settings_obj = New User_settings()
        offset_dialog = New EditOffsets()
    End Sub

    Private Sub Change_color_menu_item_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Change_color_menu_item.Click
        ColorDialog1.ShowDialog()
        Dim r As Byte = ColorDialog1.Color.R
        Dim g As Byte = ColorDialog1.Color.G
        Dim b As Byte = ColorDialog1.Color.B
        Dim r2 As String = Hex(r)
        If r2.Length = 1 Then r2 = "0" + r2
        Dim g2 As String = Hex(g)
        If g2.Length = 1 Then g2 = "0" + g2
        Dim b2 As String = Hex(b)
        If b2.Length = 1 Then b2 = "0" + b2
        user_settings_obj.font_color = r2 + g2 + b2
        'at this point, font_color = something like "FF6600"
        Render_button.PerformClick()
    End Sub

    Private Sub Change_offsets_menu_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Change_offsets_menu.Click
        offset_dialog.numerator_offset = user_settings_obj.fraction_row1_down
        offset_dialog.denominator_offset = user_settings_obj.fraction_row3_down
        offset_dialog.exponent_offset = user_settings_obj.exponent_down
        offset_dialog.subscript_offset = user_settings_obj.subscript_down
        offset_dialog.ShowDialog()
        If offset_dialog.DialogResult = DialogResult.OK _
            And IsNumeric(offset_dialog.numerator_offset) _
            And IsNumeric(offset_dialog.denominator_offset) _
            And IsNumeric(offset_dialog.exponent_offset) _
            And IsNumeric(offset_dialog.subscript_offset) Then
            user_settings_obj.fraction_row1_down = offset_dialog.numerator_offset
            user_settings_obj.fraction_row3_down = offset_dialog.denominator_offset
            user_settings_obj.exponent_down = offset_dialog.exponent_offset
            user_settings_obj.subscript_down = offset_dialog.subscript_offset
            Render_button.PerformClick()

        End If
    End Sub

    Private Sub Help_menu_item_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Help_menu_item.Click
        System.Diagnostics.Process.Start("math2html_help.htm")
    End Sub

    Private Sub Copy_to_clipboard_button_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Copy_to_clipboard_button.Click
        Clipboard.SetDataObject(html_format)
    End Sub
End Class
