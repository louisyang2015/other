Imports System.IO

Public Class Symbol_table

    Private table(1, 1) As String
    '*** The (1, 1) is just an initial size. The real size will be
    '*** set in the constructor. Each row of the table will be
    '*** a symbol and the html renderring code for that symbol
    '*** example:
    '*** table(row_number, 0) = "_pd_"
    '*** table(row_number, 1) = "&#8706;"
    '*** After the constructor, the table would be sorted
    '*** based on the column zero value so that we can use
    '*** a binary search to look up "_pd_"

    '*** This function will read the text file "file_name" and
    '*** store each line in the file as a separate string. Then
    '*** the function returns the entire array of strings.
    Private Function read_file(ByVal file_name As String) As String()
        Dim return_val(10) As String
        Dim i As Integer = 0

        Dim sr As StreamReader
        ' Open the file to read.
        sr = File.OpenText(file_name)
        ' Read each line in the file.
        ' When the end of the file is reached, "Peek" return 
        ' the value "-1".
        Dim a_line As String
        While sr.Peek <> -1
            a_line = sr.ReadLine()

            'before adding a_line to the return_val array, 
            'we might have to enlarge the return_val array
            If (i > return_val.Length - 1) Then
                ReDim Preserve return_val(i + 10)
            End If

            return_val(i) = a_line
            i = i + 1
        End While
        ' Tell user the operation is over and close the file.
        Console.WriteLine("The end of the stream has been reached.")
        sr.Close()

        'at this point, we have i lines; or we could have
        'zero lines
        '**** NOTE: we are ignoring the zero lines possibility
        'so assume we have i lines, and the highest index should
        'be i-1
        ' We need to ReDim return_val so that the "length" of
        ' return_val properly reflects the size of return_val
        ReDim Preserve return_val(i - 1)

        Return return_val
    End Function

    '*** This function will do a shell sort on "table"
    '*** table is a 2D array of strings. This function will
    '*** sort the rows fo table according to the value in 
    '*** column zero.
    Private Sub shell_sort()
        Dim increment As Integer
        increment = table.GetUpperBound(0) / 2

        While increment > 0
            ' reminder on the shell sort: not a tutorial; for 
            ' a total description, please search for "shell sort"
            ' on the Internet.
            '
            ' You have multiple lists such that:
            ' list 1: item[0], item[increment], item[2*increment], ...
            ' list 2: item[1], item[1 + increment], item[1 + 2*increment], ...
            ' list 3: item[2], item[2 + increment], item[2 + 2*increment], ...
            '       and so on
            '
            ' if you sort list 1, then list 2, then so on; then that's
            ' an inefficient implementation
            '
            ' we will sort list 1, list 2, etc. in parallel - by going
            ' through the mother list only once, from item[0] to
            ' item[count - 1]. Along the way we use insertion sort,
            ' not on the whole list but on the correct sub-list

            ' inside the following "for loop" we go through the list
            Dim i As Integer
            For i = 0 To table.GetUpperBound(0)

                ' Let say (i = 2 + 2 * increment)

                ' That means we are working on the sub list:
                ' item[2], item[2 + 1*increment], item[2 + 2*increment], ...

                ' we want to insert this item[i] into the correct
                ' location in the sublist 

                ' It's like an insertion short where we skip
                ' every few items. 


                '** at this point, the sub list: 
                '** item[i - increment], item[i - 2*increment] ...
                '** is in order.

                Dim j As Integer = i
                Dim current0 As String = table(i, 0)
                Dim current1 As String = table(i, 1)

                'now we find a spot to insert item[i] --- 
                'we look at item[i - increment], item[i - 2*increment]
                'and so on to look for an item that is smaller
                'or equal to item[i]. Items that are > item[i]
                'will be bumped down the list

                While j >= increment AndAlso _
                String.Compare(table(j - increment, 0), current0) > 0
                    table(j, 0) = table(j - increment, 0)
                    table(j, 1) = table(j - increment, 1)
                    j = j - increment
                End While

                table(j, 0) = current0
                table(j, 1) = current1

                '** at this point, the sub list: item[i], 
                '** item[i - increment], item[i - 2*increment]
                '** should be be in order

            Next

            increment = increment / 2

        End While

    End Sub

    Public Sub New(ByVal file_name As String)
        Dim lines As String()
        lines = read_file(file_name)

        ' now we need to put lines into the table 2D array

        ReDim table(lines.Length - 1, 1)

        Dim i As Integer
        For i = 0 To lines.Length - 1
            Dim tab_location As Integer
            tab_location = lines(i).IndexOf(ControlChars.Tab)
            If tab_location <> -1 Then
                table(i, 0) = lines(i).Substring(0, tab_location).Trim()
                table(i, 1) = lines(i).Substring(tab_location + 1).Trim()
            End If
        Next

        '** at this point table is filled with pairs like
        '** (_pd_, &#8706;) --- but it's unsorted. Sort the
        '** table according to the first column using shell sort
        shell_sort()

    End Sub

    '*** This function will look through column zero of table.
    '*** If the given "macro_name" string matches a string in column zero,
    '*** the function will return the value of the column one string
    '*** So look_up_macro("_pd_") will return "&#8706;"
    '*** look_up_macro("_no_such_macro_") will return an 
    '*** empty string of length zero
    '*** prior to using "look_up_macro", the table 2D array
    '*** column 0 must be sorted; It's because this function
    '*** uses the binary search in the look up process.
    Public Function look_up_macro(ByVal macro_name As String) As String
        Dim return_val As String
        return_val = New String("")

        Dim first As Integer = 0
        Dim middle As Integer = 0
        Dim last As Integer = table.GetUpperBound(0)

        While (first <= last)

            middle = (first + last) / 2
            Dim this_macro As String
            this_macro = table(middle, 0)

            If String.Compare(this_macro, macro_name) = 0 Then
                return_val = table(middle, 1)
                Exit While
            Else
                If String.Compare(macro_name, this_macro) > 0 Then
                    first = middle + 1
                Else
                    last = middle - 1
                End If
            End If
        End While

        Return return_val
    End Function

End Class
