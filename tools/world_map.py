import plotly.plotly as py
import pandas as pd

py.sign_in('mskwarek', 'gw9wh5r9h2')
#df = pd.read_csv('https://raw.githubusercontent.com/plotly/datasets/master/2014_world_gdp_with_codes.csv')


#df = pd.read_csv('/Users/mskwarek/Documents/myDig/results/countriesCnt_iso.result')
df = pd.read_csv('/Users/mskwarek/Documents/myDig/results/countriesNsManyLinesIp.result')
#df = pd.read_csv('/Users/mskwarek/Documents/myDig/results/countriesCnt_used.result')


data = [ dict(
        type = 'choropleth',
        locations = df['CODE'],
        z = df['CNT'],
        text = df['COUNTRY'],
        colorscale=[[0,"rgb(142,174,96)"],[0.05,"rgb(215,234,84)"],
                    [0.15,"rgb(222,228,63)"],[0.3,"rgb(234,179,72)"],
                    [0.6,"rgb(234,102,72)"],[1,"rgb(167,57,31)"]],
        autocolorscale = False,
        reversescale = False,
        marker = dict(
            line = dict (
                color = 'rgb(180,180,180)',
                width = 0.5
            ) ),
        colorbar = dict(
            autotick = False,
            tickprefix = '',
            title = 'Liczba domen'),
      ) ]

layout = dict(
    title = 'Lokalizacja serwerow DNS odpowiadajacych wiecej niz jednym rekordem',
    geo = dict(
        showframe = False,
        showcoastlines = False,
        projection = dict(
            type = 'Mercator'
        )
    )
)

fig = dict( data=data, layout=layout )
py.iplot( fig, validate=False, filename='d3-world-map-na-many-lines' )
