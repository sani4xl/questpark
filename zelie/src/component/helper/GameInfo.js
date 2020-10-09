class GameInfo {

    getInfo() {
        const gameInfoRaw = localStorage.getItem('gameInfo');
        if (!gameInfoRaw) {
            return {};
        }

        return JSON.parse(gameInfoRaw);
    }

    getTask() {
        return this.getInfo().task || {};
    }

    getTaskIndex() {
        return this.getInfo().taskIndex;
    }

    getGameId() {
        return localStorage.getItem('gameId');
    }

    getGems() {
        return this.getInfo().gems;
    }

    hasGems() {
        const gems = this.getGems();
        return gems && Object.keys(gems).length > 0;
    }

    getGemByName(name) {
        return this.getInfo().gems[name];
    }
}

export default GameInfo;