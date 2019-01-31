Public Class EditOffsets
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
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents numerator_text As System.Windows.Forms.TextBox
    Friend WithEvents denominator_text As System.Windows.Forms.TextBox
    Friend WithEvents Label6 As System.Windows.Forms.Label
    Friend WithEvents Done_button As System.Windows.Forms.Button
    Friend WithEvents Cancel_button As System.Windows.Forms.Button
    Friend WithEvents Label7 As System.Windows.Forms.Label
    Friend WithEvents Label8 As System.Windows.Forms.Label
    Friend WithEvents Label9 As System.Windows.Forms.Label
    Friend WithEvents Label10 As System.Windows.Forms.Label
    Friend WithEvents exponent_text As System.Windows.Forms.TextBox
    Friend WithEvents subscript_text As System.Windows.Forms.TextBox
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.numerator_text = New System.Windows.Forms.TextBox()
        Me.denominator_text = New System.Windows.Forms.TextBox()
        Me.Label6 = New System.Windows.Forms.Label()
        Me.Done_button = New System.Windows.Forms.Button()
        Me.Cancel_button = New System.Windows.Forms.Button()
        Me.Label7 = New System.Windows.Forms.Label()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.exponent_text = New System.Windows.Forms.TextBox()
        Me.subscript_text = New System.Windows.Forms.TextBox()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(12, 12)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(604, 29)
        Me.Label1.TabIndex = 0
        Me.Label1.Text = "A larger positive number will push the item down further"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label2.Location = New System.Drawing.Point(84, 130)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(133, 29)
        Me.Label2.TabIndex = 1
        Me.Label2.Text = "Numerator:"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label3.Location = New System.Drawing.Point(84, 189)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(157, 29)
        Me.Label3.TabIndex = 2
        Me.Label3.Text = "Denominator:"
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label4.Location = New System.Drawing.Point(360, 130)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(38, 29)
        Me.Label4.TabIndex = 3
        Me.Label4.Text = "ex"
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label5.Location = New System.Drawing.Point(360, 189)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(38, 29)
        Me.Label5.TabIndex = 4
        Me.Label5.Text = "ex"
        '
        'numerator_text
        '
        Me.numerator_text.Location = New System.Drawing.Point(252, 124)
        Me.numerator_text.Name = "numerator_text"
        Me.numerator_text.Size = New System.Drawing.Size(96, 35)
        Me.numerator_text.TabIndex = 5
        '
        'denominator_text
        '
        Me.denominator_text.Location = New System.Drawing.Point(252, 183)
        Me.denominator_text.Name = "denominator_text"
        Me.denominator_text.Size = New System.Drawing.Size(96, 35)
        Me.denominator_text.TabIndex = 6
        '
        'Label6
        '
        Me.Label6.AutoSize = True
        Me.Label6.Location = New System.Drawing.Point(12, 59)
        Me.Label6.Name = "Label6"
        Me.Label6.Size = New System.Drawing.Size(437, 29)
        Me.Label6.TabIndex = 7
        Me.Label6.Text = "A negative number will push the item up"
        '
        'Done_button
        '
        Me.Done_button.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.Done_button.Location = New System.Drawing.Point(468, 236)
        Me.Done_button.Name = "Done_button"
        Me.Done_button.Size = New System.Drawing.Size(120, 34)
        Me.Done_button.TabIndex = 8
        Me.Done_button.Text = "Done"
        '
        'Cancel_button
        '
        Me.Cancel_button.DialogResult = System.Windows.Forms.DialogResult.Cancel
        Me.Cancel_button.Location = New System.Drawing.Point(468, 307)
        Me.Cancel_button.Name = "Cancel_button"
        Me.Cancel_button.Size = New System.Drawing.Size(120, 33)
        Me.Cancel_button.TabIndex = 9
        Me.Cancel_button.Text = "Cancel"
        '
        'Label7
        '
        Me.Label7.AutoSize = True
        Me.Label7.Location = New System.Drawing.Point(84, 248)
        Me.Label7.Name = "Label7"
        Me.Label7.Size = New System.Drawing.Size(120, 29)
        Me.Label7.TabIndex = 10
        Me.Label7.Text = "Exponent:"
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(84, 307)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(120, 29)
        Me.Label8.TabIndex = 11
        Me.Label8.Text = "Subscript:"
        '
        'exponent_text
        '
        Me.exponent_text.Location = New System.Drawing.Point(252, 242)
        Me.exponent_text.Name = "exponent_text"
        Me.exponent_text.Size = New System.Drawing.Size(96, 35)
        Me.exponent_text.TabIndex = 12
        '
        'subscript_text
        '
        Me.subscript_text.Location = New System.Drawing.Point(252, 301)
        Me.subscript_text.Name = "subscript_text"
        Me.subscript_text.Size = New System.Drawing.Size(96, 35)
        Me.subscript_text.TabIndex = 13
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(360, 248)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(38, 29)
        Me.Label9.TabIndex = 14
        Me.Label9.Text = "ex"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Location = New System.Drawing.Point(360, 307)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(38, 29)
        Me.Label10.TabIndex = 15
        Me.Label10.Text = "ex"
        '
        'EditOffsets
        '
        Me.AcceptButton = Me.Done_button
        Me.AutoScaleBaseSize = New System.Drawing.Size(12, 28)
        Me.CancelButton = Me.Cancel_button
        Me.ClientSize = New System.Drawing.Size(687, 371)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.subscript_text)
        Me.Controls.Add(Me.exponent_text)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.Label7)
        Me.Controls.Add(Me.Cancel_button)
        Me.Controls.Add(Me.Done_button)
        Me.Controls.Add(Me.Label6)
        Me.Controls.Add(Me.denominator_text)
        Me.Controls.Add(Me.numerator_text)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.Label4)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Font = New System.Drawing.Font("Microsoft Sans Serif", 12.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "EditOffsets"
        Me.Text = "Edit Style Sheet Offsets"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

#End Region

    Public Property numerator_offset() As String
        Get
            Return numerator_text.Text
        End Get

        Set(ByVal value As String)
            numerator_text.Text = value
        End Set
    End Property

    Public Property denominator_offset() As String
        Get
            Return denominator_text.Text
        End Get

        Set(ByVal value As String)
            denominator_text.Text = value
        End Set
    End Property

    Public Property exponent_offset() As String
        Get
            Return exponent_text.Text
        End Get

        Set(ByVal value As String)
            exponent_text.Text = value
        End Set
    End Property

    Public Property subscript_offset() As String
        Get
            Return subscript_text.Text
        End Get

        Set(ByVal value As String)
            subscript_text.Text = value
        End Set
    End Property

End Class
