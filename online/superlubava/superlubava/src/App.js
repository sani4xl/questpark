import React from 'react';
import { Switch, Route } from 'react-router-dom';

import Home from './Home';
import Gauntlet from './Gauntlet';
import Menu from './Menu';

import './App.css';

function App() {
  return (
    <div className="App">
      <Switch>
          <Route exact path='/' component={Home} />
          <Route exact path='/gauntlet' component={Gauntlet} />
          <Route exact path='/menu' component={Menu} />
      </Switch>
    </div>
  );
}

export default App;
