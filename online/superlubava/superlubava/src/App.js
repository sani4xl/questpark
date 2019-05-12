import React from 'react';
import Home from './Home';
import { Switch, Route } from 'react-router-dom';
import Gauntlet from './Gauntlet';

import './App.css';

function App() {
  return (
    <div className="App">
      <Switch>
          <Route exact path='/' component={Home} />
          <Route exact path='/gauntlet' component={Gauntlet} />
      </Switch>
    </div>
  );
}

export default App;
