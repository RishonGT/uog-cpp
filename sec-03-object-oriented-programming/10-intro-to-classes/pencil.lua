function pencil()
  return pandoc.RawInline('html', '&#9998;&nbsp;')
end

return {
  {
    -- This looks for the shortcode {{< pencil >}}
    ['pencil'] = pencil
  }
}