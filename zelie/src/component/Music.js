import React from 'react';
import themeMusic from '../res/theme.mp3';

class Music extends React.Component {


    constructor(props) {
        super(props);
        this.state = {
        }
        
        this.audio = new Audio(themeMusic);
    }

    componentDidMount() {
        this.audio.play();
    }

    
    render() {
    
        return (
          <div>
              
          </div>
          );
        }
}

export default Music;